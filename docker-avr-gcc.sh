#!/bin/bash

DOCKER_IMAGE=alpine-avr-gcc
MCU=attiny1614
FILES=$@

echo $DOCKER_IMAGE
echo $MCU
echo $FILES

docker run --rm -v $(pwd):/var/avr $DOCKER_IMAGE \
  avr-g++ -c -mmcu=$MCU -Os -flto -ffunction-sections $FILES

#avr-g++ -mmcu=attiny1614 -flto -o main.elf main.o
#avr-objdump -S main.elf > main.S
