#include <errno.h>
#include <sys/types.h>

// TODO: このfileの扱われ方.
void _exit(void) {
  while (1) __asm__("hlt");
}

// MEMO: 
// program_break_endはInitializeHeap()によって初期化される
caddr_t program_break, program_break_end;

caddr_t sbrk(int incr) {
  if (program_break == 0 || program_break + incr >= program_break_end) {
    errno = ENOMEM;
    return (caddr_t)-1;
  }

  caddr_t prev_break = program_break;
  program_break += incr;
  return prev_break;
}

int getpid(void) {
  return 1;
}

int kill(int pid, int sig) {
  errno = EINVAL;
  return -1;
}
