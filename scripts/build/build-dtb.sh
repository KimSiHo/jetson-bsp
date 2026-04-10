#!/bin/bash

set -e

for arg in "$@"; do
    eval "$arg"
done


step_start() {
    STEP_NAME="$1"
    echo "========================== START: $STEP_NAME =========================="
    STEP_BEGIN=$(date +%s)
}

step_end() {
    STEP_END=$(date +%s)
    ELAPSED=$((STEP_END - STEP_BEGIN))
    echo "========================== DONE: $STEP_NAME ( ${ELAPSED}s ) =========================="
    echo
}

step_start "DTB Build"

cd "$L4T_DIR/source"
export KERNEL_HEADERS=$PWD/kernel/kernel-jammy-src
make dtbs

cp kernel-devicetree/generic-dts/dtbs/* \
   "$L4T_DIR/kernel/dtb/"

step_end

step_start "Copy DTBO to rootfs"

sudo install --owner=root --group=root --mode=644 -CDv \
    "$L4T_DIR/kernel/dtb/"*.dtb* \
    "$L4T_DIR/rootfs/boot/"

step_end

echo "========================== BUILD DTB FINISHED =========================="
play -nq -t alsa synth 1 sine 300
