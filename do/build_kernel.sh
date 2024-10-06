set -ex

ROOT=$PWD
BUILD=$ROOT/build

rm -rf $BUILD/kernel
mkdir -p $BUILD/kernel
cd $BUILD/kernel

mkdir freestnd-c-hdrs
tar xf $ROOT/deps/freestnd-c-hdrs-0bsd.tar.gz -C freestnd-c-hdrs --strip-components=1

CFLAGS="
-std=c11
-fno-stack-protector
-fno-stack-check
-fno-lto
-fno-PIC
-ffunction-sections
-fdata-sections
-mno-red-zone
-mno-80387
-mno-mmx
-mno-sse
-mno-sse2
-mcmodel=kernel
-isystem freestnd-c-hdrs
-I$BUILD/limine/build/include
"

zig build-lib -target x86_64-freestanding -O ReleaseSafe \
  --name kernel \
  -cflags $CFLAGS -- \
  $(ls $ROOT/src/*.c)

zig cc \
  -target x86_64-freestanding -O2 \
  -o kernel \
  -z max-page-size=0x1000 \
  -Wl,--gc-sections \
  -e kmain \
  -T$ROOT/src/kernel.ld \
  libkernel.a
