# Toolchain for AVR 8-bit microcontrollers
A docker packaged AVR toolchain, consisting of a modern version of `avr-gcc`,
`avr-libc` (from Arduino nightly builds), and `avrdude` flashing tool with "jtag2updi" config.

### Update 2020-07-23:
Blink example confirmed working with the following setup/versions:
- GCC 10.2.0 (with avrxmega3.xn patch, and binutils 2.34)
- Arduino nightly from 2020-06-24 (only avr-libc is used)
- avrdude 6.3
- tinyAVR 1-series attiny1614

See [example-tiny1614-blink](https://github.com/vladbelous/toolchain-avr/tree/master/example-tiny1614-blink) for an example Makefile project using this toolchain in docker.

TODO:
- Confirm if this is working for older popular attiny (attiny85, attiny841) and atmega (atmega328, atmega32u4) chips.
- ISP flashing (pass appropriate USB device to docker container)
- Check out [avr-libc3](https://github.com/stevenj/avr-libc3) as an alternative to the outdated `avr-libc`, with modern AVR chip support. This helps us avoid the hack with Arduino nightly/hourly builds.

## License
Unless otherwise stated, all files are licensed under the terms described in the `LICENSE` file.

## Initial setup
Download and place into `alpine-avr-gcc` directory:
- GCC (e.g. https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.xz)
- binutils (e.g. https://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.xz)
- Arduino nightly/hourly build (e.g. https://www.arduino.cc/download.php?f=/arduino-nightly-linux64.tar.xz - name file as `arduino-nightly-YYYYMMDD.tar.xz`). We really only need `avr-libc` from it.
- avrdude (e.g. http://download.savannah.gnu.org/releases/avrdude/avrdude-6.3.tar.gz)

If using any different versions, make sure to update the hard-coded versions in Dockerfile.

If GCC or Arduino versions differ, probably worth verifying if `ldscript-avrxmega3.xn.patch` (see Dockerfile) applies cleanly, or whether it's still necessary.

Build toolchain docker image:
```
cd alpine-avr-gcc
docker build -t alpine-avr-gcc .
```

## Usage example
See [example-tiny1614-blink](https://github.com/vladbelous/toolchain-avr/tree/master/example-tiny1614-blink) for a basic complete blink project.
