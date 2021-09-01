#pragma once

#include "graphics.hpp"

class Console {
 public:
  //
  static const int kRows = 25, kColumns = 80;

  // proto宣言だけのconstructor.
  Console(PixelWriter& writer,
      const PixelColor& fg_color, const PixelColor& bg_color);
  // interface
  void PutString(const char* s);

 private:
  // interface
  void Newline();

  PixelWriter& writer_;
  const PixelColor fg_color_, bg_color_;
  // MEOM: console全体の文字を記憶するbuffer.
  char buffer_[kRows][kColumns + 1];
  int cursor_row_, cursor_column_;
};