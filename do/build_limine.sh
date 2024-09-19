set -ex

ROOT=$PWD
BUILD=$ROOT/build

rm -rf $BUILD/limine
mkdir -p $BUILD/limine

cd $BUILD/limine
tar xf $ROOT/deps/limine-8.0.13.tar.gz --strip-components=1
./configure --prefix=$PWD/build \
  --enable-uefi-x86-64 \
  --enable-bios
make -j8 install
