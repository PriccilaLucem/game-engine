#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

// ============================================
// CONFIGURATION
// ============================================

typedef struct PoolBlock PoolBlock;
typedef struct ChunkHeader ChunkHeader;
typedef struct DynamicPool DynamicPool;

// ============================================
// STRUCTURES
// ============================================

struct ChunkHeader {
    PoolBlock* owner_block;
    ChunkHeader* next;
};

struct PoolBlock {
    PoolBlock* next;
    ChunkHeader* free_list;
    void* memory;
    size_t chunk_size;
    size_t chunk_count;
};

struct DynamicPool {
    PoolBlock* blocks;
    size_t chunk_size;
    size_t chunk_count_per_block;
    size_t alignment;

    // Statistics
    size_t total_blocks;
    size_t total_chunks;
    size_t used_chunks;
    size_t peak_usage;

#ifdef POOL_THREAD_SAFE
    void* lock;
#endif
};

// ============================================
// PUBLIC FUNCTIONS
// ============================================

void dynamic_pool_init(DynamicPool* pool, size_t chunk_size, size_t chunk_count_per_block, size_t alignment);
void* dynamic_pool_alloc(DynamicPool* pool);
void dynamic_pool_free(DynamicPool* pool, void* ptr);
void dynamic_pool_destroy(DynamicPool* pool);
void dynamic_pool_print_stats(const DynamicPool* pool);

// ============================================
// CONVENIENCE MACROS
// ============================================

#define POOL_ALLOC(pool, Type) ((Type*)dynamic_pool_alloc(pool))
#define POOL_FREE(pool, ptr) dynamic_pool_free(pool, ptr)

// ============================================
// TYPE-SPECIFIC POOL MACROS
// ============================================

#define POOL_DECLARE(PoolName, Type)                              \
    void PoolName##_Init(size_t chunk_count_per_block, size_t alignment); \
    Type* PoolName##_Alloc(void);                                 \
    void PoolName##_Free(Type* ptr);                              \
    void PoolName##_Destroy(void);                                \
    void PoolName##_PrintStats(void);

#define POOL_DEFINE(PoolName, Type)                               \
    static DynamicPool PoolName##_pool;                           \
    void PoolName##_Init(size_t chunk_count_per_block, size_t alignment) { \
        dynamic_pool_init(&PoolName##_pool, sizeof(Type), chunk_count_per_block, alignment); \
    }                                                             \
    Type* PoolName##_Alloc(void) {                                \
        return (Type*)dynamic_pool_alloc(&PoolName##_pool);       \
    }                                                             \
    void PoolName##_Free(Type* ptr) {                             \
        dynamic_pool_free(&PoolName##_pool, ptr);                 \
    }                                                             \
    void PoolName##_Destroy(void) {                               \
        dynamic_pool_destroy(&PoolName##_pool);                   \
    }                                                             \
    void PoolName##_PrintStats(void) {                            \
        dynamic_pool_print_stats(&PoolName##_pool);               \
    }


#endif // POOL_ALLOCATOR_H
