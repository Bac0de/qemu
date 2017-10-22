#!/bin/bash

~/project/qemu/build/x86_64-softmmu/qemu-system-x86_64 -boot d -hda ~/project/winhd.img -m 4096  -enable-kvm -cpu Nehalem -smp cores=4 -vga qxl
