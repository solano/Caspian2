#!/bin/sh
set -e
SCRIPT=$(readlink -f "$0")
SCRIPTDIR=$(dirname "$SCRIPT")
. $SCRIPTDIR/config.sh

mkdir -p isodir/boot/grub

cp $BUILD_DIR/kernel/kernel.bin isodir/boot/kernel.bin
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Caspian2" {
    multiboot /boot/kernel.bin
}
EOF
grub-mkrescue -o Caspian2.iso isodir
rm -rf isodir
