/**
 * @file console.cpp
 *
 * コンソール描画のプログラムを集めたファイル．
 */

#include "console.hpp"

#include <cstring>
#include "font.hpp"
#include "layer.hpp"

// MEMO: constructor定義
Console::Console(const PixelColor& fg_color, const PixelColor& bg_color)
    : writer_{nullptr}, fg_color_{fg_color}, bg_color_{bg_color},
      buffer_{}, cursor_row_{0}, cursor_column_{0} {
}

void Console::PutString(const char* s) {
  while (*s) {
    if (*s == '\n') {
      Newline();
    } else if (cursor_column_ < kColumns - 1) { // kColumns - 1を超えた分は描画しない方針
      WriteAscii(*writer_, 8 * cursor_column_, 16 * cursor_row_, *s, fg_color_);
      // MEMO: scrollに備えてカーソル位置の文字を記憶しておく
      buffer_[cursor_row_][cursor_column_] = *s;
      ++cursor_column_;
    }
    ++s;
  }
  if (layer_manager) {
    layer_manager->Draw();
  }
}

// SetWriterの具体的な実装.
void Console::SetWriter(PixelWriter* writer) {
  if (writer == writer_) {
    return;
  }
  writer_ = writer;
  // MEMO: ここでどうして再描画する必要あり？
  Refresh();
}

void Console::Newline() {
  cursor_column_ = 0;
  if (cursor_row_ < kRows - 1) {
    ++cursor_row_;
  } else {
    // consoleの画面サイズが大きくなると結構重そう
    for (int y = 0; y < 16 * kRows; ++y) {
      for (int x = 0; x < 8 * kColumns; ++x) {
        writer_->Write(x, y, bg_color_);
      }
    }
    for (int row = 0; row < kRows - 1; ++row) {
      // 全てのrowを1つ上のbufferにcopy.
      memcpy(buffer_[row], buffer_[row + 1], kColumns + 1);
      WriteString(*writer_, 0, 16 * row, buffer_[row], fg_color_);
    }
    // 新しく生成したlineを0で埋める
    memset(buffer_[kRows - 1], 0, kColumns + 1);
  }
}

// buffer_に溜めている文字を、再描画.
void Console::Refresh() {
  for (int row = 0; row < kRows; ++row) {
    WriteString(*writer_, 0, 16 * row, buffer_[row], fg_color_);
  }
}