# Toolchain in docker image
DOCKER=docker
TOOLCHAIN_IMAGE=alpine-avr-gcc

PWD=$(shell pwd)
DOCKER_RUN=$(DOCKER) run --rm -v $(PWD):/var/avr $(TOOLCHAIN_IMAGE)

CPP=$(DOCKER_RUN) avr-g++
OBJCOPY=$(DOCKER_RUN) avr-objcopy
OBJDUMP=$(DOCKER_RUN) avr-objdump
AVRDUDE=$(DOCKER_RUN) avrdude

# MCU
MCU=attiny1614

# Compile options
CPPFLAGS=-c -Os -mmcu=$(MCU)

# Link flags
# TODO: some issue with linker script related to
# the __REGION_DATA_ORIGIN__ symbol.
# See: https://www.avrfreaks.net/forum/solved-compiling-attiny1607-or-other-0-series1-series-avr-gcc 
LINKFLAGS=-mmcu=$(MCU) -T my-avrxmega3.x
# Consider: -flto -ffunction-sections

blink.elf: main.cpp
	$(CPP) $(CPPFLAGS) main.cpp
	$(CPP) $(LINKFLAGS) -o $@ main.o

blink.hex: blink.elf
	$(OBJCOPY) -j .text -j .data -j .rodata \
        -O ihex $< $@

blink.S: blink.elf
	$(OBJDUMP) -S $< > $@


# TODO: add flashing with avrdude.
