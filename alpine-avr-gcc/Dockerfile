FROM alpine:3.12 AS build-env

RUN apk --update add wget build-base perl texinfo

ENV PREFIX /opt/avr-toolchain
ENV PATH "${PREFIX}/bin:${PATH}"

ENV PREFIX_AVRDUDE /opt/avrdude
ENV PATH "${PREFIX_AVRDUDE}/bin:${PATH}"

# https://ftp.gnu.org/gnu/binutils/binutils-XXX.tar.xz
ENV PKG_BINUTILS_VER 2.34
ENV PKG_BINUTILS binutils-${PKG_BINUTILS_VER}

# https://ftp.gnu.org/gnu/gcc-XXX/gcc-XXX.tar.xz
ENV PKG_GCC_VER 10.1.0
ENV PKG_GCC gcc-${PKG_GCC_VER}

# Expecting arduino-nightly-{YYYYMMDD}.tar.xz
ENV PKG_ARDUINO_DATE 20200624
ENV PKG_ARDUINO arduino-nightly-${PKG_ARDUINO_DATE}

# http://download.savannah.gnu.org/releases/avrdude/
ENV PKG_AVRDUDE_VER 6.3
ENV PKG_AVRDUDE avrdude-${PKG_AVRDUDE_VER}

# Test settings for target binaries
ENV CFLAGS "-Os"
ENV CXXFLAGS "-Os"

# If using native-only
#ENV CFLAGS "-Os -march=native"
#ENV CXXFLAGS "-Os -march=native"


# Build as non-root user
WORKDIR /var/build
RUN addgroup avr \
  && adduser -D -G avr avr \
  && chown avr:avr . \
  && mkdir -p ${PREFIX} \
  && chown -R avr:avr ${PREFIX} \
  && mkdir -p ${PREFIX_AVRDUDE} \
  && chown -R avr:avr ${PREFIX_AVRDUDE}
USER avr

# Build and install binutils
COPY ${PKG_BINUTILS}.tar.xz /var/build/
RUN tar -xf ${PKG_BINUTILS}.tar.xz \
  && cd ${PKG_BINUTILS} \
    && mkdir build-out \
    && cd build-out \
      && ../configure --prefix=$PREFIX --target=avr --disable-nls \
      && make -j$(nproc) \
      && make install \
      && cd ../.. \
  && rm -rf ${PKG_BINUTILS}

# Build and install GCC
COPY ${PKG_GCC}.tar.xz /var/build/
RUN tar -xf ${PKG_GCC}.tar.xz \
  && cd ${PKG_GCC} \
    && sed -i 's/sha512sum --check/sha512sum -c/' \
        contrib/download_prerequisites \
    && ./contrib/download_prerequisites \
    && mkdir build-out \
    && cd build-out \
      && ../configure --prefix=$PREFIX --target=avr \
         --enable-languages=c,c++ \
         --with-dwarf2 --disable-nls --disable-plugin \
      && make -j$(nproc) \
      && make install \
      && strip ${PREFIX}/libexec/gcc/avr/*/cc1 \
      && strip ${PREFIX}/libexec/gcc/avr/*/cc1plus \
      && strip ${PREFIX}/libexec/gcc/avr/*/lto1 \
      && cd ../.. \
  && rm -rf ${PKG_GCC}

# Patch ldscripts/avrxmega3.xn due to a DATA_REGION_ORIGIN issue.
# See: https://www.avrfreaks.net/forum/solved-compiling-attiny1607-or-other-0-series1-series-avr-gcc 
COPY ldscript-avrxmega3.xn.patch /var/build/
RUN cd ${PREFIX}/avr/lib/ldscripts/ \
  && cp -v /var/build/ldscript-avrxmega3.xn.patch . \
  && patch -p0 --backup avrxmega3.xn < ldscript-avrxmega3.xn.patch

# Take avr-libc files from arduino-nightly package
COPY ${PKG_ARDUINO}.tar.xz /var/build/
RUN tar -xf ${PKG_ARDUINO}.tar.xz \
  && cd arduino-nightly/hardware/tools/avr \
    && cp -rv avr/include ${PREFIX}/avr/ \
    && cp -rv avr/lib/avr* ${PREFIX}/avr/lib/ \
    && cp -v  avr/lib/crt*.o ${PREFIX}/avr/lib/ \
    && cp -v  avr/lib/lib*.a ${PREFIX}/avr/lib/ \
    && cp -rv avr/lib/tiny-stack ${PREFIX}/avr/lib/ \
  && cd /var/build \
  && rm -rf arduino-nightly

# Build and install avrdude
# - install extra packages for avrdude
USER root
RUN apk --update add bison flex libusb-dev
USER avr
# - build and install
COPY ${PKG_AVRDUDE}.tar.gz /var/build/
RUN tar -xf ${PKG_AVRDUDE}.tar.gz \
  && cd ${PKG_AVRDUDE} \
    && mkdir build-out \
    && cd build-out \
      && ../configure --prefix=${PREFIX_AVRDUDE} \
                      --disable-parport \
      && make -j$(nproc) \
      && make install \
      && cd ../.. \
  && rm -rf ${PKG_AVRDUDE}


# Final environment:
FROM alpine:3.12

RUN apk --update add libusb

ENV PREFIX /opt/avr-toolchain
ENV PATH "${PREFIX}/bin:${PATH}"

ENV PREFIX_AVRDUDE /opt/avrdude
ENV PATH "${PREFIX_AVRDUDE}/bin:${PATH}"

COPY --from=build-env $PREFIX $PREFIX
COPY --from=build-env $PREFIX_AVRDUDE $PREFIX_AVRDUDE

WORKDIR /var/avr
RUN addgroup avr \
  && adduser -D -G avr avr \
  && addgroup avr dialout \
  && chown avr:avr .
USER avr
