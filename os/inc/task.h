#ifndef __TASK__
#define __TASK__

#include <stdint.h>
#include <stdbool.h>

#define TDESC_ERR -1
#define TASK_QUEUE_SIZE 16

typedef int16_t task_desc_t;
typedef bool (*task_cb_t)(void*);

typedef struct {
  void* ctx;
  task_cb_t cb;
  bool repeat;
} task_t;

task_desc_t task_start(task_t* task);
void task_run();
void task_stop(task_desc_t desc);
void task_wait(task_desc_t desc);

#endif