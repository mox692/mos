#! /bin/bash

clang++  -I/home/vscode/osbook/devenv/x86_64-elf/include/c++/v1 -I/home/vscode/osbook/devenv/x86_64-elf/include -I/home/vscode/osbook/devenv/x86_64-elf/include/freetype2     -I/home/vscode/edk2/MdePkg/Include -I/home/vscode/edk2/MdePkg/Include/X64     -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS     -DEFIAPI='__attribute__((ms_abi))' -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone -fno-exceptions -fno-rtti -std=c++17 -stdlib=libc++ -c -o main.o main.cpp

ld.lld -L/home/vscode/osbook/devenv/x86_64-elf/lib -o kernel.elf main.o