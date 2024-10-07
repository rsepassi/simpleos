set -ex

ROOT=$PWD
BUILD=$ROOT/build

IMG=${1:-$BUILD/boot/xos.img}
DISK=${2:-/tmp/xosdata.qcow2}

if [ ! -f $DISK ]
then
  qemu-img create -f qcow2 $DISK 1G
fi

qemu-system-x86_64 \
  -accel kvm \
  -bios /usr/share/OVMF/OVMF.fd \
  -cpu host -smp 2 -m 4G \
  -serial stdio \
  -nic user,hostfwd=::8080-:22,model=virtio-net-pci \
  -drive file=$IMG,format=raw,index=0,media=disk \
  -drive file=$DISK,format=qcow2,index=1,media=disk
