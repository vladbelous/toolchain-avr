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
CPPFLAGS=-c -Os -mmcu=$(MCU) -flto

# Link flags
LINKFLAGS=-mmcu=$(MCU) -flto
# Consider: -flto -ffunction-sections

blink.hex: blink.elf
	$(OBJCOPY) -j .text -j .data -j .rodata \
        -O ihex $< $@

blink.S: blink.elf
	$(OBJDUMP) -S $< > $@

blink.elf: main.cpp
	$(CPP) $(CPPFLAGS) main.cpp
	$(CPP) $(LINKFLAGS) -o $@ main.o


# TODO: add flashing with avrdude.
