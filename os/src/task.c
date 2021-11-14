#include <task.h>
#include <mem.h>
#include <timer.h>

static task_t* _tasks[TASK_QUEUE_SIZE];

task_desc_t task_start(task_t* task) {
  uint8_t desc;

  for(desc = 0; desc < TASK_QUEUE_SIZE; desc++) {
    if (_tasks[desc] == NULL) {
      break;
    }
  }

  if (desc == TASK_QUEUE_SIZE) {
    return TDESC_ERR;
  }

  _tasks[desc] = task;

  return desc;
}

void task_run() {
  for(uint8_t i = 0; i < TASK_QUEUE_SIZE; i++) {
    if (_tasks[i] != NULL) {
      if(_tasks[i]->cb(_tasks[i]->ctx) && !_tasks[i]->repeat) {
        _tasks[i] = NULL;
      }
    }
  }
}

void task_stop(task_desc_t desc) {
  _tasks[desc] = NULL;
}

void task_wait(task_desc_t desc) {
  while(_tasks[desc] != NULL) {
    yield();
  }
}
