#ifndef MEMORYSTUFF_H
#define MEMORYSTUFF_H

typedef void* (*memory_allocate_t)(u64);

typedef struct {
  u8 *cursor;
  u8 *end;
  u64 total_requested;
  u64 highest_reached;
  u64 size;
  u8 *data;
} pool_t;

pool_t allocate_pool(u64 size) {
  pool_t pool;
  
  pool.data = malloc(size);
  pool.size = size;
  
  pool.cursor = pool.data;
  pool.end = pool.data + size;
  pool.total_requested = 0;
  pool.highest_reached = 0;
  
  return pool;
}
void free_pool(pool_t *pool) {
  assert(pool);
  
  if(pool->data)
    free(pool->data);
  
  memset(pool, 0, sizeof(pool_t));
}

void reset_pool(pool_t *pool) {
  assert(pool);
  
  pool->cursor = pool->data;
  
  if(pool->total_requested > pool->highest_reached)
    pool->highest_reached = pool->total_requested;
  
  pool->total_requested = 0;
}

void* pool_allocate(pool_t *pool, u64 allocation_size) {
  assert(pool);
  
  pool->total_requested += allocation_size;
  
  if(pool->data + pool->size - pool->cursor < allocation_size)
    return malloc(allocation_size);
  
  u8 *memory = pool->cursor;
  pool->cursor += allocation_size;
  
  return memory;
}

#define TEMPORARY_POOL_SIZE 8192
pool_t temporary_pool;

void* temp_allocate(u64 size) {
  return pool_allocate(&temporary_pool, size);
}

#endif // MEMORYSTUFF_H
