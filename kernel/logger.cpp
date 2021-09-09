#include "logger.hpp"

#include <cstddef>
#include <cstdio>

#include "console.hpp"

// 無名namespace: https://marycore.jp/prog/cpp/unnamed-namespace/
// 他のファイルから値を参照することができなくなる.
// staticとの違いは？？
namespace {
  LogLevel log_level = kWarn;
}

extern Console* console;

void SetLogLevel(LogLevel level) {
  log_level = level;
}

int Log(LogLevel level, const char* format, ...) {
  if (level > log_level) {
    return 0;
  }

  va_list ap;
  int result;
  char s[1024];

  va_start(ap, format);
  result = vsprintf(s, format, ap);
  va_end(ap);

  console->PutString(s);
  return result;
}
