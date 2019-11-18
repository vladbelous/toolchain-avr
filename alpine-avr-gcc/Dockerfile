FROM alpine:3.10 AS build-env

RUN apk --update add wget build-base

ENV PREFIX /opt/avr-toolchain
ENV PATH "${PREFIX}/bin:${PATH}"

# https://ftp.gnu.org/gnu/binutils/binutils-2.33.1.tar.xz
ENV PKG_BINUTILS binutils-2.33.1
# https://ftp.gnu.org/gnu/gcc/gcc-9.2.0/gcc-9.2.0.tar.xz
ENV PKG_GCC gcc-9.2.0

# Expecting arduino-nightly-{YYYYMMDD}.tar.xz
ENV PKG_ARDUINO arduino-nightly
ENV PKG_ARDUINO_DATE 20191113

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
  && chown -R avr:avr ${PREFIX}
USER avr

# Build and install binutils
COPY ${PKG_BINUTILS}.tar.xz /var/build/${PKG_BINUTILS}.tar.xz 
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
COPY ${PKG_GCC}.tar.xz /var/build/${PKG_GCC}.tar.xz 
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

# Take avr-libc files from arduino-nightly package
COPY ${PKG_ARDUINO}-${PKG_ARDUINO_DATE}.tar.xz /var/build/${PKG_ARDUINO}.tar.xz
RUN tar -xf ${PKG_ARDUINO}.tar.xz \
  && cd ${PKG_ARDUINO}/hardware/tools/avr \
    && cp -r avr/include ${PREFIX}/avr/ \
    && cp -r avr/lib/avr* ${PREFIX}/avr/lib/ \
    && cp avr/lib/crt*.o ${PREFIX}/avr/lib/ \
    && cp avr/lib/lib*.a ${PREFIX}/avr/lib/ \
    && cp -r avr/lib/tiny-stack ${PREFIX}/avr/lib/ \
  && cd /var/build \
  && rm -rf ${PKG_ARDUINO}

# Optionally, copy a few device-specs files missing from 9.2.0:
RUN tar -xf ${PKG_ARDUINO}.tar.xz \
  && cd ${PKG_ARDUINO}/hardware/tools/avr \
  && cd lib/gcc/avr/7.3.0/device-specs \
  && ( for fn in 1608 1609 3208 3209 324pb 4808 4809 808 809; do \
        cp specs-atmega${fn} ${PREFIX}/lib/gcc/avr/9.2.0/device-specs/ ; \
       done \
     ) \
  && ( for fn in 102 104 1604 1606 1607 202 204 402 404 406 804 806 807; do \
        cp specs-attiny${fn} ${PREFIX}/lib/gcc/avr/9.2.0/device-specs/ ; \
       done \
     ) \
  && cd /var/build \
  && rm -rf ${PKG_ARDUINO}


# Final environment:
FROM alpine:3.10

ENV PREFIX /opt/avr-toolchain
ENV PATH "${PREFIX}/bin:${PATH}"
COPY --from=build-env $PREFIX $PREFIX

WORKDIR /var/avr
RUN addgroup avr \
  && adduser -D -G avr avr \
  && chown avr:avr .
USER avr
