/**
 * @file interrupt.cpp
 *
 * 割り込み用のプログラムを集めたファイル．
 */

#include "interrupt.hpp"

#include "asmfunc.h"
#include "segment.hpp"

// Interrupt Discriptor Table.
// 全部で256種のInterrupt Discriptor(割り込み要因と、それに対するハンドラを記述した記述子)
// を格納できる.
// ref: 165
std::array<InterruptDescriptor, 256> idt;

// IDTのメンバを受け取って、その記述子のInterruptDescriptorメンバを埋める.
void SetIDTEntry(InterruptDescriptor& desc,
                 InterruptDescriptorAttribute attr,
                 // 第一引数のdescに対応したhandlerのaddr.
                 // ハンドラのアドレスがそのまま渡ってくる(なんでoffsetという名前にした??)
                 uint64_t offset,
                 uint16_t segment_selector) {
  desc.attr = attr;
  desc.offset_low = offset & 0xffffu;
  desc.offset_middle = (offset >> 16) & 0xffffu;
  desc.offset_high = offset >> 32;
  desc.segment_selector = segment_selector;
}

// MEMO: 0xfee000b0に書き込むことでINTの終了を通知できる.
void NotifyEndOfInterrupt() {
  volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);
  *end_of_interrupt = 0;
}

namespace {
  std::deque<Message>* msg_queue;

  __attribute__((interrupt))
  void IntHandlerXHCI(InterruptFrame* frame) {
    msg_queue->push_back(Message{Message::kInterruptXHCI});
    NotifyEndOfInterrupt();
  }
}

void InitializeInterrupt(std::deque<Message>* msg_queue) {
  ::msg_queue = msg_queue;

  SetIDTEntry(idt[InterruptVector::kXHCI],
              MakeIDTAttr(DescriptorType::kInterruptGate, 0),
              reinterpret_cast<uint64_t>(IntHandlerXHCI),
              kKernelCS);
  LoadIDT(sizeof(idt) - 1, reinterpret_cast<uintptr_t>(&idt[0]));
}

