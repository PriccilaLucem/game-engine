# Pool Allocator

A high-performance memory pool allocator for game engines with dynamic block allocation and optional thread safety.

## Features

- **Dynamic Growth**: Automatically creates new blocks when needed
- **Memory Alignment**: Configurable alignment for optimal performance
- **Thread Safety**: Optional mutex protection (compile with `POOL_THREAD_SAFE`)
- **Statistics**: Built-in memory usage tracking
- **Zero Fragmentation**: Fixed-size chunks eliminate fragmentation

## Usage

### Basic Example

```c
#include "pool/pool.h"

// Initialize pool for 64-byte objects, 100 per block, 16-byte aligned
DynamicPool pool;
dynamic_pool_init(&pool, 64, 100, 16);

// Allocate memory
void* ptr = dynamic_pool_alloc(&pool);

// Use the memory...

// Free memory
dynamic_pool_free(&pool, ptr);

// Cleanup
dynamic_pool_destroy(&pool);
```

### With Type Safety

```c
typedef struct {
    float x, y, z;
    int id;
} GameObject;

DynamicPool game_objects;
dynamic_pool_init(&game_objects, sizeof(GameObject), 50, alignof(GameObject));

// Type-safe allocation
GameObject* obj = POOL_ALLOC(&game_objects, GameObject);
obj->x = 10.0f;

// Free when done
POOL_FREE(&game_objects, obj);
```

## API Reference

### Functions

| Function | Description |
|----------|-------------|
| `dynamic_pool_init()` | Initialize pool with chunk size, count per block, and alignment |
| `dynamic_pool_alloc()` | Allocate one chunk from the pool |
| `dynamic_pool_free()` | Return chunk to the pool |
| `dynamic_pool_destroy()` | Free all memory and cleanup |
| `dynamic_pool_print_stats()` | Print usage statistics |

### Macros

| Macro | Description |
|-------|-------------|
| `POOL_ALLOC(pool, Type)` | Type-safe allocation |
| `POOL_FREE(pool, ptr)` | Free memory chunk |

### TYPE-SPECIFIC POOL MACROS

Generate type-specific pools with automatic sizing and alignment:

```c
// Generate pool type for GameObjects
DECLARE_TYPED_POOL(GameObject);
DEFINE_TYPED_POOL(GameObject);

// Usage
GameObjectPool pool;
gameobject_pool_init(&pool, 100);  // 100 objects per block
GameObject* obj = gameobject_pool_alloc(&pool);
gameobject_pool_free(&pool, obj);
gameobject_pool_destroy(&pool);
```

## Configuration

### Thread Safety
```c
#define POOL_THREAD_SAFE
#include "pool/pool.h"
```

### Alignment
- Default: Natural alignment of chunk size
- Custom: Specify alignment in `dynamic_pool_init()`
- Common values: 4, 8, 16, 32, 64 bytes

## Performance

- **O(1)** allocation and deallocation
- **Cache-friendly**: Contiguous memory blocks
- **Low overhead**: ~16 bytes per chunk header
- **No fragmentation**: Fixed-size allocations

## Memory Layout

```
Block 1: [Header|Chunk][Header|Chunk][Header|Chunk]...
Block 2: [Header|Chunk][Header|Chunk][Header|Chunk]...
```

Each chunk has a small header containing:
- Pointer to owner block
- Next free chunk pointer

## Best Practices

1. **Size pools appropriately**: Match chunk size to your objects
2. **Use alignment**: Align to cache line boundaries for performance
3. **Monitor statistics**: Use `dynamic_pool_print_stats()` to optimize
4. **Pool similar objects**: Group objects of same size together
5. **Pre-allocate**: Initialize with expected peak usage

## Example Output

```
=== Pool Stats ===
Chunk size: 64 bytes (aligned)
Blocks: 2
Chunks total: 200
Used: 150 | Free: 50
Peak usage: 180
```