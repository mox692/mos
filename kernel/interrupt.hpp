/**
 * @file interrupt.hpp
 *
 * 割り込み用のプログラムを集めたファイル．
 */

#pragma once

#include <array>
#include <cstdint>
#include <deque>

#include "x86_descriptor.hpp"
#include "message.hpp"

// 割り込み記述子の属性を表現.
// bit fieldを用いているので、全体のsizeとしては16bit.
union InterruptDescriptorAttribute {
  uint16_t data;
  struct {
    uint16_t interrupt_stack_table : 3;
    uint16_t : 5;
    DescriptorType type : 4;                  // trap gate or interrupt gate
    uint16_t : 1;
    uint16_t descriptor_privilege_level : 2;  // DPL(割り込みハンドラの実行権限)の設定
    uint16_t present : 1;
  } __attribute__((packed)) bits; // !! bit field
} __attribute__((packed));        // !! No align

// Interrupt Descriptorの定義.
// ref: p166.
struct InterruptDescriptor {
  uint16_t offset_low;                // 割り込みハンドラの格納場所(64bit addr)のうちの16bit
  uint16_t segment_selector;          // 割り込みが発生した際のコードセグメント(csレジスタの値)を格納.(復帰する際に戻るため??ref: p166)
  InterruptDescriptorAttribute attr;  // 割り込み記述子の属性を表現
  uint16_t offset_middle;             // 割り込みハンドラの格納場所(64bit addr)のうちの16bit
  uint32_t offset_high;               // 割り込みハンドラの格納場所(64bit addr)のうちの32bit(highだけ32bit)
  uint32_t reserved;
} __attribute__((packed));

extern std::array<InterruptDescriptor, 256> idt;

// InterruptDescriptorAttributeのメンバを埋める関数.
constexpr InterruptDescriptorAttribute MakeIDTAttr(
    DescriptorType type,
    uint8_t descriptor_privilege_level,
    bool present = true,
    uint8_t interrupt_stack_table = 0) {
  InterruptDescriptorAttribute attr{};
  attr.bits.interrupt_stack_table = interrupt_stack_table;
  attr.bits.type = type;
  attr.bits.descriptor_privilege_level = descriptor_privilege_level;
  attr.bits.present = present;
  // MEMO: 値を返してる事に注意!(ダングリングpointerにはならない)
  return attr;
}

void SetIDTEntry(InterruptDescriptor& desc,
                 InterruptDescriptorAttribute attr,
                 uint64_t offset,
                 uint16_t segment_selector);

// #@@range_begin(vector_numbers)
class InterruptVector {
 public:
  enum Number {
    kXHCI = 0x40,
    kLAPICTimer = 0x41,
  };
};

struct InterruptFrame {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
};

void NotifyEndOfInterrupt();

void InitializeInterrupt(std::deque<Message>* msg_queue);