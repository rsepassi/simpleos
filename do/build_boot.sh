set -ex

ROOT=$PWD
BUILD=$ROOT/build

rm -rf $BUILD/boot
mkdir -p $BUILD/boot
cd $BUILD/boot

IMG=xos.img
FAT=$IMG.fat

KERNEL=$BUILD/kernel/kernel
EFI=$BUILD/limine/build/share/limine/BOOTX64.EFI
BIOS=$BUILD/limine/build/share/limine/limine-bios.sys
LIMINE=$BUILD/limine/build/bin/limine
LIMINE_CFG=$ROOT/src/limine.conf

# Create and populate the FAT32 filesystem
dd if=/dev/zero of=$FAT bs=62M count=1
mformat -F -i $FAT ::
mmd -i $FAT ::/boot ::/EFI ::/EFI/BOOT
mcopy -i $FAT $EFI ::/EFI/BOOT/$LIMINE_EFI_BIN
mcopy -i $FAT $BIOS ::/boot/limine-bios.sys
mcopy -i $FAT $LIMINE_CFG ::/boot/limine.conf
mcopy -i $FAT $KERNEL ::/boot/kernel

# Make the GPT container
dd if=/dev/zero of=$IMG bs=64M count=1
parted -s $IMG -- \
  unit MiB \
  mklabel gpt \
  mkpart primary fat32 1MiB 63MiB \
  set 1 esp on

# Copy in the FAT32 partition
dd if=$FAT of=$IMG bs=1M seek=1 conv=notrunc

# Install Limine BIOS
$LIMINE bios-install $IMG
