TARG=kernel.elf
OBJS=main.o
CFLGS=-Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
		-fno-exceptions -fno-rtti -std=c++17 -c
LDFLGS=--entry KernelMain -z norelro --image-base 0x100000 --static \

$(TARG): $(OBJS)
	clang++ $(CFLGS) -o $(OBJS) main.cpp
	ld.lld $(LDFLGS) -o $(TARG) $(OBJS)

clean:
	rm *.o $(TARG)