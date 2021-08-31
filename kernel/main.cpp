#include <cstdint>
#include <cstddef>
#include "frame_buffer_config.hpp"
// #@@range_begin(includes)
#include "graphics.hpp"
#include "font.hpp"
// #@@range_end(includes)

// #@@range_begin(placement_new)
// TODO: 調べる
void* operator new(size_t size, void* buf) {
  return buf;
}

void operator delete(void* obj) noexcept {
}

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

// const参照型
extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
  switch (frame_buffer_config.pixel_format) {
    case kPixelRGBResv8BitPerColor:
      // MEMO: 配置newでインスタンスのためのmemoryを確保.
      pixel_writer = new(pixel_writer_buf)
        RGBResv8BitPerColorPixelWriter{frame_buffer_config};
      break;
    case kPixelBGRResv8BitPerColor:
      pixel_writer = new(pixel_writer_buf)
        BGRResv8BitPerColorPixelWriter{frame_buffer_config};
      break;
  }
  // write all pixel white.
  for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
    for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
      pixel_writer->Write(x, y, {255, 255, 255});
    }
  }
  // write green reqtangle (100 < x < 300, 100 < y < 200).
  for (int x = 0; x < 200; ++x) {
    for (int y = 0; y < 100; ++y) {
      pixel_writer->Write(x + 200, y + 400, {255, 0, 0});
    }
  }

  // #@@range_begin(write_aa)
  WriteAscii(*pixel_writer, 50, 50, 'A', {0, 0, 0});
  WriteAscii(*pixel_writer, 58, 50, 'A', {0, 0, 0});
  // #@@range_end(write_aa)

  while (1) __asm__("hlt");
}
