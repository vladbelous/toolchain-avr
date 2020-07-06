# Toolchain in docker image
DOCKER=docker
TOOLCHAIN_IMAGE=alpine-avr-gcc

PROJECT_DIR=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
DOCKER_RUN=$(DOCKER) run --rm -v $(PROJECT_DIR):/var/avr $(TOOLCHAIN_IMAGE)

CPP=$(DOCKER_RUN) avr-g++
OBJCOPY=$(DOCKER_RUN) avr-objcopy
OBJDUMP=$(DOCKER_RUN) avr-objdump
SIZE=$(DOCKER_RUN) avr-size
AVRDUDE=$(DOCKER_RUN) avrdude

# Compile flags
CPPFLAGS=-c -Os -mmcu=$(MCU) -flto

# Link flags
# Consider: -flto -ffunction-sections
LINKFLAGS=-mmcu=$(MCU) -flto

# ----------------------

# MCU
MCU=attiny1614

SRCS = src/main.cpp
OBJS = $(patsubst src/%.cpp,build/%.o,$(SRCS))

build/blink.hex: build/blink.elf
	$(OBJCOPY) -j .text -j .data -j .rodata \
        -O ihex $< $@
	$(SIZE) $<

build/blink.s: build/blink.elf
	$(OBJDUMP) -S $< > $@

build/blink.elf: $(OBJS)
	$(CPP) $(LINKFLAGS) -o $@ $(OBJS)

build/%.o: src/%.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<

.PHONY: clean
clean:
	-rm build/*


# TODO: add flashing with avrdude.
