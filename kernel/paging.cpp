#include "paging.hpp"

#include <array>

#include "asmfunc.h"

namespace {
  const uint64_t kPageSize4K = 4096;
  const uint64_t kPageSize2M = 512 * kPageSize4K;
  const uint64_t kPageSize1G = 512 * kPageSize2M;

  alignas(kPageSize4K) std::array<uint64_t, 512> pml4_table;
  alignas(kPageSize4K) std::array<uint64_t, 512> pdp_table;
  alignas(kPageSize4K)
    std::array<std::array<uint64_t, 512>, kPageDirectoryCount> page_directory;
}

// MEMO: 簡易的(3層構造?)なpagetableを設定する,CR3レジスタにrootのaddrをいれる
void SetupIdentityPageTable() {
  // root dirの先頭要素にその1つ下のtableの先頭要素のaddrを入れている.
  // TODO: なんで先頭要素だけしか使ってない? bit和は？
  pml4_table[0] = reinterpret_cast<uint64_t>(&pdp_table[0]) | 0x003;
  for (int i_pdpt = 0; i_pdpt < page_directory.size(); ++i_pdpt) {
    // pdp_tableの各要素にその下層のtableのaddrを格納してる
    // (こっちはloopで各要素の下を入れている.)
    pdp_table[i_pdpt] = reinterpret_cast<uint64_t>(&page_directory[i_pdpt]) | 0x003;
    for (int i_pd = 0; i_pd < 512; ++i_pd) {
      // これは物理ADDR??
      page_directory[i_pdpt][i_pd] = i_pdpt * kPageSize1G + i_pd * kPageSize2M | 0x083;
    }
  }

  // rootのtableの先頭addrをCR3にsetする
  SetCR3(reinterpret_cast<uint64_t>(&pml4_table[0]));
}

void InitializePaging() {
  SetupIdentityPageTable();
}