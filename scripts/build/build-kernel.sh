#!/bin/bash

set -e

for arg in "$@"; do
    eval "$arg"
done

if [ "$PHASE" = "dev" ]; then
    DEFCONFIG="defconfig defconfig_debug.config"
else
    DEFCONFIG="defconfig"
fi

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

step_start "1 Kernel Build"
cd "$L4T_DIR/source"
make -C kernel KERNEL_DEF_CONFIG="$DEFCONFIG"
step_end

step_start "2 Kernel Install (in-tree modules)"
export INSTALL_MOD_PATH=$L4T_DIR/rootfs
sudo -E make install -C kernel
step_end

step_start "3 Copy Kernel Image"
cp "$L4T_DIR/source/kernel/kernel-jammy-src/arch/arm64/boot/Image" "$L4T_DIR/kernel/Image"
step_end

step_start "4 OOT Module Build"
cd "$L4T_DIR/source"
export KERNEL_HEADERS=$PWD/kernel/kernel-jammy-src
make modules
step_end

step_start "5 OOT Module Install"
export INSTALL_MOD_PATH=$L4T_DIR/rootfs
sudo -E make modules_install
step_end

step_start "6 depmod"
cd $L4T_DIR
if [ "$PHASE" = "dev" ]; then
    sudo depmod -b rootfs 5.15.148-debug-tegra
else
    sudo depmod -b rootfs 5.15.148-tegra
fi
step_end

step_start "7 Update Initrd Rootfs Source"
cd "$L4T_DIR"
sudo ./tools/l4t_update_initrd.sh
step_end

echo "========================== BUILD KERNEL FINISHED =========================="
play -nq -t alsa synth 1 sine 300
