set -ex

ROOT=$PWD
BUILD=$ROOT/build

IMG=$BUILD/boot/xos.img

qemu-system-x86_64 \
  -accel kvm \
  -m 4G -smp 2 \
  -nic user,hostfwd=::8080-:22,model=virtio-net-pci \
  -drive file=$IMG,format=raw,index=0
