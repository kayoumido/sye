#!/bin/sh
qemu/arm-softmmu/qemu-system-arm $1 -s -M vexpress-a15 -kernel u-boot/u-boot -m 1024 -serial mon:stdio -nographic -net nic -net user,tftp=.


