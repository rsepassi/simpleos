set -ex

ROOT=$PWD
BUILD=$ROOT/build

LIMINE_CFG=${1:-$ROOT/src/limine.conf}
KERNEL=${2:-$BUILD/kernel/kernel}
LIMINEDIR=${3:-$BUILD/limine/build}

rm -rf $BUILD/boot
mkdir -p $BUILD/boot
cd $BUILD/boot

IMG=xos.img
FAT=$IMG.fat

EFI=$LIMINEDIR/share/limine/BOOTX64.EFI
BIOS=$LIMINEDIR/share/limine/limine-bios.sys
LIMINE=$LIMINEDIR/bin/limine

# Create and populate the FAT16 filesystem
dd if=/dev/zero of=$FAT bs=6M count=1
mformat -i $FAT ::
mmd -i $FAT ::/boot ::/EFI ::/EFI/BOOT
mcopy -i $FAT $EFI ::/EFI/BOOT/$LIMINE_EFI_BIN
mcopy -i $FAT $BIOS ::/boot/limine-bios.sys
mcopy -i $FAT $LIMINE_CFG ::/boot/limine.conf
mcopy -i $FAT $KERNEL ::/boot/kernel

# Make the GPT container
dd if=/dev/zero of=$IMG bs=8M count=1
parted -s $IMG -- \
  unit MiB \
  mklabel gpt \
  mkpart primary fat32 1MiB 7MiB \
  set 1 esp on

# Copy in the FAT16 partition
dd if=$FAT of=$IMG bs=1M seek=1 conv=notrunc

# Install Limine BIOS
$LIMINE bios-install $IMG
