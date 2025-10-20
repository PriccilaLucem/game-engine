#include "pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "../logs/log.h"

#ifdef POOL_THREAD_SAFE
#include <pthread.h>
#endif

// ============================================
// HELPER FUNCTION: ALIGNMENT
// ============================================
static inline size_t align_up(size_t value, size_t alignment) {
    return (value + (alignment - 1)) & ~(alignment - 1);
}

// ============================================
// MEMORY BLOCK
// ============================================
static PoolBlock* create_block(size_t chunk_size, size_t chunk_count, size_t alignment) {
    PoolBlock* block = malloc(sizeof(PoolBlock));
    if (!block) return NULL;

    size_t chunk_with_header = sizeof(ChunkHeader) + chunk_size;
    size_t total_size = chunk_with_header * chunk_count;

    void* mem = NULL;
#if defined(_MSC_VER)
    mem = _aligned_malloc(total_size, alignment);
    if (!mem) {
        free(block);
        return NULL;
    }
#else
    if (posix_memalign(&mem, alignment, total_size) != 0) {
        free(block);
        return NULL;
    }
#endif

    block->memory = mem;
    block->chunk_size = chunk_size;
    block->chunk_count = chunk_count;
    block->next = NULL;

    // Create linked list of chunks
    uint8_t* ptr = (uint8_t*)mem;
    for (size_t i = 0; i < chunk_count; i++) {
        ChunkHeader* header = (ChunkHeader*)(ptr + i * chunk_with_header);
        header->owner_block = block;
        header->next = (i < chunk_count - 1)
            ? (ChunkHeader*)(ptr + (i + 1) * chunk_with_header)
            : NULL;
    }

    block->free_list = (ChunkHeader*)mem;
    return block;
}

// ============================================
// INITIALIZATION
// ============================================
void dynamic_pool_init(DynamicPool* pool, size_t chunk_size, size_t chunk_count_per_block, size_t alignment) {
    pool->chunk_size = align_up(chunk_size, alignment);
    pool->chunk_count_per_block = chunk_count_per_block;
    pool->alignment = alignment;
    pool->blocks = create_block(pool->chunk_size, pool->chunk_count_per_block, alignment);

    pool->total_blocks = 1;
    pool->total_chunks = chunk_count_per_block;
    pool->used_chunks = 0;
    pool->peak_usage = 0;

#ifdef POOL_THREAD_SAFE
    pthread_mutex_t* mtx = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mtx, NULL);
    pool->lock = mtx;
#endif
}

// ============================================
// ALLOCATION
// ============================================
void* dynamic_pool_alloc(DynamicPool* pool) {
#ifdef POOL_THREAD_SAFE
    pthread_mutex_lock((pthread_mutex_t*)pool->lock);
#endif

    PoolBlock* block = pool->blocks;

    while (block && !block->free_list)
        block = block->next;

    if (!block) {
        PoolBlock* new_block = create_block(pool->chunk_size, pool->chunk_count_per_block, pool->alignment);
        new_block->next = pool->blocks;
        pool->blocks = new_block;
        block = new_block;

        pool->total_blocks++;
        pool->total_chunks += pool->chunk_count_per_block;
    }

    ChunkHeader* header = block->free_list;
    block->free_list = header->next;

    pool->used_chunks++;
    if (pool->used_chunks > pool->peak_usage)
        pool->peak_usage = pool->used_chunks;

#ifdef POOL_THREAD_SAFE
    pthread_mutex_unlock((pthread_mutex_t*)pool->lock);
#endif

    return (void*)((uint8_t*)header + sizeof(ChunkHeader));
}

// ============================================
// DEALLOCATION
// ============================================
void dynamic_pool_free(DynamicPool* pool, void* ptr) {
    if (!ptr) return;

#ifdef POOL_THREAD_SAFE
    pthread_mutex_lock((pthread_mutex_t*)pool->lock);
#endif

    ChunkHeader* header = (ChunkHeader*)((uint8_t*)ptr - sizeof(ChunkHeader));
    PoolBlock* block = header->owner_block;

    header->next = block->free_list;
    block->free_list = header;

    if (pool->used_chunks > 0)
        pool->used_chunks--;

#ifdef POOL_THREAD_SAFE
    pthread_mutex_unlock((pthread_mutex_t*)pool->lock);
#endif
}

// ============================================
// DESTRUCTION
// ============================================
void dynamic_pool_destroy(DynamicPool* pool) {
    PoolBlock* block = pool->blocks;
    while (block) {
        PoolBlock* next = block->next;
#if defined(_MSC_VER)
        _aligned_free(block->memory);
#else
        free(block->memory);
#endif
        free(block);
        block = next;
    }

#ifdef POOL_THREAD_SAFE
    pthread_mutex_destroy((pthread_mutex_t*)pool->lock);
    free(pool->lock);
#endif

    memset(pool, 0, sizeof(DynamicPool));
}

// ============================================
// STATISTICS
// ============================================
void dynamic_pool_print_stats(const DynamicPool* pool) {
    log_message(LOG_LEVEL_INFO, "=== Pool Stats ===\n", __FILE__);
    log_message(LOG_LEVEL_INFO, "Chunk size: %zu bytes (aligned)\n", pool->chunk_size, __FILE__);
    log_message(LOG_LEVEL_INFO, "Blocks: %zu\n", pool->total_blocks, __FILE__);
    log_message(LOG_LEVEL_INFO, "Chunks total: %zu\n", pool->total_chunks, __FILE__);
    log_message(LOG_LEVEL_INFO, "Used: %zu | Free: %zu\n", pool->used_chunks, pool->total_chunks - pool->used_chunks, __FILE__);
    log_message(LOG_LEVEL_INFO, "Peak usage: %zu\n", pool->peak_usage, __FILE__);
}
