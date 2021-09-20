#pragma once

#include <stdint.h>

enum PixelFormat {
  kPixelRGBResv8BitPerColor,
  kPixelBGRResv8BitPerColor,
};

struct FrameBufferConfig {
  /// Base address of graphics linear frame buffer.
  /// Offset zero in FrameBufferBase represents the upper left pixel of the display.
  uint8_t* frame_buffer;
  /// Defines the number of pixel elements per video memory line.
  uint32_t pixels_per_scan_line;
  // 水平方向分解能
  uint32_t horizontal_resolution;
  // 垂直方向分解能
  uint32_t vertical_resolution;
  enum PixelFormat pixel_format;
};
