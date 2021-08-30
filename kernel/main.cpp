#include <cstdint>

extern "C" void KernelMain(uint64_t frame_buffer_base, uint64_t frame_buffer_size) {
  // bootloaderからframe_bufferへのアドレスとsizeを受け取り、
  // それらの値を変更していく.
  uint8_t *frame_buffer = reinterpret_cast<uint_fast8_t*>(frame_buffer_base);
  for (uint64_t i = 0; i < frame_buffer_size; ++i) {
    frame_buffer[i] = 100;
  }
  while (1) __asm__("hlt");
}
