#pragma once

#include <stdint.h>

// 構造体定義はUEFI specificな部分. UEFI規格で定められてる定義を独自に構造体定義とした.
// ref: edk2/MdePkg/Include/Uefi/UefiSpec.h
// ref: p57
struct MemoryMap {
  unsigned long long buffer_size;
  // memory mapの中身の先頭アドレス.
  // 内部的にはEFI_MEMORY_DESCRIPTORの配列になっている(ref:edk2/MdePkg/Include/Uefi/UefiSpec.h)
  void* buffer;
  // MemoryMap全体のsizeを表す(EFI_MEMORY_DESCRIPTORエントリのサイズではない!)
  unsigned long long map_size;
  unsigned long long map_key;
  // EFI_MEMORY_DESCRIPTORのサイズ.(UEFIがupdateされるにつれ、ここが変動しうる)
  unsigned long long descriptor_size;
  // EFI_MEMORY_DESCRIPTORのサイズ.(上とも関連し、恐らくupdateされることを見越してこのfieldを入れた?)
  uint32_t descriptor_version;
};

// MEMO: EFI_MEMORY_DESCRIPTORの定義を独自構造体として再定義したもの
struct MemoryDescriptor {
  uint32_t type;
  uintptr_t physical_start;
  uintptr_t virtual_start;
  uint64_t number_of_pages;
  uint64_t attribute;
};

#ifdef __cplusplus
enum class MemoryType {
  kEfiReservedMemoryType,
  kEfiLoaderCode,
  kEfiLoaderData,
  kEfiBootServicesCode,
  kEfiBootServicesData,
  kEfiRuntimeServicesCode,
  kEfiRuntimeServicesData,
  kEfiConventionalMemory,
  kEfiUnusableMemory,
  kEfiACPIReclaimMemory,
  kEfiACPIMemoryNVS,
  kEfiMemoryMappedIO,
  kEfiMemoryMappedIOPortSpace,
  kEfiPalCode,
  kEfiPersistentMemory,
  kEfiMaxMemoryType
};

inline bool operator==(uint32_t lhs, MemoryType rhs) {
  return lhs == static_cast<uint32_t>(rhs);
}

inline bool operator==(MemoryType lhs, uint32_t rhs) {
  return rhs == lhs;
}

inline bool IsAvailable(MemoryType memory_type) {
  return
    memory_type == MemoryType::kEfiBootServicesCode ||
    memory_type == MemoryType::kEfiBootServicesData ||
    memory_type == MemoryType::kEfiConventionalMemory;
}

const int kUEFIPageSize = 4096;
#endif
