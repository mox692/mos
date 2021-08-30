TARG=kernel.elf
OBJS=main.o
CXX=clang++
LD=ld.lld
CFLGSSS=-Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
		-fno-exceptions -fno-rtti -std=c++17 -stdlib=libc++
LLDFLGS=$(LDFLAGS) --entry KernelMain -z norelro --image-base 0x100000 --static \


$(TARG): $(OBJS)
	$(CXX) $(CPPFLAGS) $(CFLGSSS) -c -o $(OBJS) main.cpp
	$(LD) $(LLDFLGS) -o $(TARG) $(OBJS)

clean:
	rm *.o $(TARG)