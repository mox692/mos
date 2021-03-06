#include "timer.hpp"

#include "acpi.hpp"
#include "interrupt.hpp"
#include "task.hpp"

namespace {
  const uint32_t kCountMax = 0xffffffffu;
  volatile uint32_t& lvt_timer = *reinterpret_cast<uint32_t*>(0xfee00320);
  volatile uint32_t& initial_count = *reinterpret_cast<uint32_t*>(0xfee00380);
  volatile uint32_t& current_count = *reinterpret_cast<uint32_t*>(0xfee00390);
  volatile uint32_t& divide_config = *reinterpret_cast<uint32_t*>(0xfee003e0);
}

// MEMO: p227を参照.
void InitializeLAPICTimer(std::deque<Message>& msg_queue) {
  timer_manager = new TimerManager{msg_queue};

  divide_config = 0b1011; // divide 1:1
  lvt_timer = 0b001 << 16; // masked, one-shot

  StartLAPICTimer();
  acpi::WaitMilliseconds(100);
  const auto elapsed = LAPICTimerElapsed();
  StopLAPICTimer();

  lapic_timer_freq = static_cast<unsigned long>(elapsed) * 10;

  divide_config = 0b1011; // divide 1:1
  lvt_timer = (0b010 << 16) | InterruptVector::kLAPICTimer; // not-masked, periodic
  initial_count = lapic_timer_freq / kTimerFreq;
}

void StartLAPICTimer() {
  initial_count = kCountMax;
}

uint32_t LAPICTimerElapsed() {
  // MEMO: Max - 現在 を計算している(経過ごとに値はどんどん小さくなっていく?)
  return kCountMax - current_count;
}

void StopLAPICTimer() {
  // MEMO: 初期値を0にしてしまう.(これがstopになってるのかは謎)
  initial_count = 0;
}

Timer::Timer(unsigned long timeout, int value)
    : timeout_{timeout}, value_{value} {
}

TimerManager::TimerManager(std::deque<Message>& msg_queue)
    : msg_queue_{msg_queue} {
  timers_.push(Timer{std::numeric_limits<unsigned long>::max(), -1});
}

// MEMO: P277
void TimerManager::AddTimer(const Timer& timer) {
  timers_.push(timer);
}

// MEMO: countを実際にする処理
//       ref: 274
bool TimerManager::Tick() {
  ++tick_;

  bool task_timer_timeout = false;
  while (true) {
    const auto& t = timers_.top();
    if (t.Timeout() > tick_) {
      break;
    }

    // MEMO: timeoutしたtimerがTaskTimerかどうか判定する.
    if (t.Value() == kTaskTimerValue) {
      task_timer_timeout = true;
      timers_.pop();
      timers_.push(Timer{tick_ + kTaskTimerPeriod, kTaskTimerValue});
      // MEMO: task switchのtickは通知しない
      continue;
    }

    Message m{Message::kTimerTimeout};
    m.arg.timer.timeout = t.Timeout();
    m.arg.timer.value = t.Value();
    msg_queue_.push_back(m);

    timers_.pop();
  }

  return task_timer_timeout;
}

TimerManager* timer_manager;
unsigned long lapic_timer_freq;

// MEMO: timerがタイムアウトした際の処理(ハンドラ)
void LAPICTimerOnInterrupt() {
  const bool task_timer_timeout = timer_manager->Tick();
  NotifyEndOfInterrupt();

  if (task_timer_timeout) {
    SwitchTask();
  }
}
