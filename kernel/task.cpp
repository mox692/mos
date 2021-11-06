#include "task.hpp"

#include "asmfunc.h"
#include "timer.hpp"

// #@@range_begin(switchtask)
alignas(16) TaskContext task_b_ctx, task_a_ctx;

namespace {
  TaskContext* current_task;
}

void SwitchTask() {
  TaskContext* old_current_task = current_task;
  if (current_task == &task_a_ctx) {
    current_task = &task_b_ctx;
  } else {
    current_task = &task_a_ctx;
  }
  SwitchContext(current_task, old_current_task);
}

void InitializeTask() {
  current_task = &task_a_ctx;

  __asm__("cli");
  // MEMO: task切り替えようにTimerを追加
  // Timerについては、p277~あたり
  timer_manager->AddTimer(
      // MEMO: tickはじまってるので、現在のtick + 0.02秒にしておく!
      Timer{timer_manager->CurrentTick() + kTaskTimerPeriod, kTaskTimerValue});
  __asm__("sti");
}
// #@@range_end(inittask)
