TARGET = kernel.elf
OBJS = main.o

CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS)

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $<


#### old ####
# TARG=kernel.elf
# OBJS=main.o
# CXX=clang++
# LD=ld.lld
# CXXFLAGS += -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
# 		-fno-exceptions -fno-rtti -std=c++17
# LDFLGS +=--entry KernelMain -z norelro --image-base 0x100000 --static \


# $(TARG): $(OBJS)
# 	$(LD) $(LDFLGS) -o $@ $^

# %.o: %.cpp
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

# clean:
# 	rm *.o $(TARG)