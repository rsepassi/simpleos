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
-nostdinc
-ffreestanding
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
-m64
-march=x86-64
-isystem freestnd-c-hdrs
-I$BUILD/limine/build/include
"

SRCS="
$(ls $ROOT/src/*.c)
"
for src in $SRCS
do
  clang $CFLAGS -c $src -o "$(basename $src).o"
done

ld.lld -o kernel \
  -nostdlib \
  -static \
  -z max-page-size=0x1000 \
  -gc-sections \
  -T$ROOT/src/kernel.ld \
  *.o
