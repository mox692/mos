#include <cstdint>
#include <cstddef>
#include "frame_buffer_config.hpp"

// #@@range_begin(font_a)
const uint8_t kFontA[16] = {
  0b00000000, //
  0b00011000, //    **
  0b00011000, //    **
  0b00011000, //    **
  0b00011000, //    **
  0b00100100, //   *  *
  0b00100100, //   *  *
  0b00100100, //   *  *
  0b00100100, //   *  *
  0b01111110, //  ******
  0b01000010, //  *    *
  0b01000010, //  *    *
  0b01000010, //  *    *
  0b11100111, // ***  ***
  0b00000000, //
  0b00000000, //
};
// #@@range_end(font_a)

struct PixelColor {
  // 8bit*8bit*8bit(予備の8bit*)で1pixel当たり4byteのdata構造を持つ.
  uint8_t r, g, b;
};

// #@@range_begin(pixel_writer)
class PixelWriter {
 public:
  // constructor
  PixelWriter(const FrameBufferConfig& config) : config_{config} {
  }
  // destructor
  virtual ~PixelWriter() = default;
  // interface
  // frame bufferを起点にして、xyで表現されるピクセルのPixelColorをcで表現されるように変更する
  virtual void Write(int x, int y, const PixelColor& c) = 0;

 protected:
  uint8_t* PixelAt(int x, int y) {
    return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
  }

 private:
  const FrameBufferConfig& config_;
};
// #@@range_end(pixel_writer)

// #@@range_begin(derived_pixel_writer)
// MEMO: PixelWriterを継承
class RGBResv8BitPerColorPixelWriter : public PixelWriter {
 public:
  // MEMO: PixelWriterのconstructor(PixelWriter)を使用.
  using PixelWriter::PixelWriter;

  // MEMO: interfaceの実装
  virtual void Write(int x, int y, const PixelColor& c) override {
    auto p = PixelAt(x, y);
    p[0] = c.r;
    p[1] = c.g;
    p[2] = c.b;
  }
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
 public:
  using PixelWriter::PixelWriter;

  virtual void Write(int x, int y, const PixelColor& c) override {
    auto p = PixelAt(x, y);
    p[0] = c.b;
    p[1] = c.g;
    p[2] = c.r;
  }
};
// #@@range_end(derived_pixel_writer)

// #@@range_begin(write_ascii)
void WriteAscii(PixelWriter& writer, int x, int y, char c, const PixelColor& color) {
  if (c != 'A') {
    return;
  }
  for (int dy = 0; dy < 16; ++dy) {
    for (int dx = 0; dx < 8; ++dx) {
      if ((kFontA[dy] << dx) & 0x80u) {
        writer.Write(x + dx, y + dy, color);
      }
    }
  }
}
// #@@range_end(write_ascii)

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
