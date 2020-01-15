#!/bin/sh

rm yummyArmOS yummyArmJFFS2

./mkimage -A arm -O linux -T kernel -C none -a a0008000 -e a0008000 -n 'yummyArmOS 0.0.0.1' -d yummyArmOS.img uImage

mv uImage kernel/boot

mkfs.jffs2 -e 0x20000 -d kernel -p -o yummyArmJFFS2

dd of=yummyArmOS bs=1k conv=notrunc if=u-boot.bin
dd of=yummyArmOS bs=1k conv=notrunc seek=180 if=yummyArmJFFS2

qemu-system-arm -M connex -pflash yummyArmOS -nographic
