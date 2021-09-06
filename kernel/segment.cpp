#include "segment.hpp"

#include "asmfunc.h"

// #@@range_begin(gdt_definition)
namespace {
  std::array<SegmentDescriptor, 3> gdt;
}
// #@@range_end(gdt_definition)

// #@@range_begin(setup_segm_function)
void SetCodeSegment(SegmentDescriptor& desc,
                    DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit) {
  desc.data = 0;

  desc.bits.base_low = base & 0xffffu;
  desc.bits.base_middle = (base >> 16) & 0xffu;
  desc.bits.base_high = (base >> 24) & 0xffu;

  desc.bits.limit_low = limit & 0xffffu;
  desc.bits.limit_high = (limit >> 16) & 0xfu;

  desc.bits.type = type;
  desc.bits.system_segment = 1; // 1: code & data segment
  desc.bits.descriptor_privilege_level = descriptor_privilege_level;
  desc.bits.present = 1;
  desc.bits.available = 0;
  desc.bits.long_mode = 1;
  desc.bits.default_operation_size = 0; // should be 0 when long_mode == 1
  desc.bits.granularity = 1;
}

void SetDataSegment(SegmentDescriptor& desc,
                    DescriptorType type,
                    unsigned int descriptor_privilege_level,
                    uint32_t base,
                    uint32_t limit) {
  SetCodeSegment(desc, type, descriptor_privilege_level, base, limit);
  desc.bits.long_mode = 0;
  desc.bits.default_operation_size = 1; // 32-bit stack segment
}

// gdtの各要素に具体的なdescを設定し、cpuにloadする.
void SetupSegments() {
  gdt[0].data = 0;
  // MEMO: 全てbase 0だと、segment領域が被ってそうだが...。
  //       このあたりはpagingを理解するとわかる?
  //       Segmentはとりあえずgdtに設定しておいて、
  //       その後にgdtに対してリニアアドレスを割り振ってるのかな.
  SetCodeSegment(gdt[1], DescriptorType::kExecuteRead, 0, 0, 0xfffff);
  SetDataSegment(gdt[2], DescriptorType::kReadWrite, 0, 0, 0xfffff);
  // これを実行した瞬間にgdtがBIOS由来のものからOSのものに変更される.
  LoadGDT(sizeof(gdt) - 1, reinterpret_cast<uintptr_t>(&gdt[0]));
}
// #@@range_end(setup_segm_function)