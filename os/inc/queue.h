#ifndef __QUEUE__
#define __QUEUE__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint32_t read;
  uint32_t write;
  uint32_t size;
  void* buffer;
} queue_t;

void queue_push_circular_overwrite_uint32(queue_t *queue, uint32_t e);
bool queue_read_circular_uint32(queue_t *queue, uint32_t *e);

#endif