TARG=kernel.elf
OBJS=main.o
CXX=clang++
LD=ld.lld
CXXFLAGS += -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
		-fno-exceptions -fno-rtti -std=c++17 -stdlib=libc++
LDFLGS +=--entry KernelMain -z norelro --image-base 0x100000 --static \


$(TARG): $(OBJS)
	$(LD) $(LDFLGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean:
	rm *.o $(TARG)