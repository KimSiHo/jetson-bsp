#!/bin/bash

set -e

for arg in "$@"; do
    eval "$arg"
done

regen=${regen:-true}
boot=${boot:-nfs}

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

if [ "$regen" = "true" ]; then
    step_start "0 Remove system.img"

    sudo rm -f \
        "$L4T_DIR/bootloader/system.img" \
        "$L4T_DIR/bootloader/system.img.raw"

    step_end
fi

step_start "1 Flash Jetson ($boot)"

cd "$L4T_DIR"

case "$boot" in
    nvme)
        sudo bash -x ./tools/kernel_flash/l4t_initrd_flash.sh \
            --external-device nvme0n1 \
            -p "-c ./bootloader/generic/cfg/flash_t234_qspi.xml" \
            -c ./tools/kernel_flash/flash_l4t_t234_nvme.xml \
            --network usb0 \
            jetson-orin-nano-devkit \
            nvme0n1p1
        ;;

    nfs)
        # 1. Generate QSPI images
        sudo ./tools/kernel_flash/l4t_initrd_flash.sh \
            --no-flash \
            -p " -c bootloader/generic/cfg/flash_t234_qspi.xml --no-systemimg" \
            jetson-orin-nano-devkit internal

        # 2. Generate NFS boot images
        sudo ./tools/kernel_flash/l4t_initrd_flash.sh \
            --no-flash \
            -p "-N 192.168.45.96:/home/sihokim/projects/proximity-system/jetson-bsp/Linux_for_Tegra/rootfs" \
            --external-device nvme0n1 \
            -c tools/kernel_flash/flash_l4t_external.xml \
            --external-only \
            --append \
            jetson-orin-nano-devkit eth0

        # 3. Flash
        sudo ./tools/kernel_flash/l4t_initrd_flash.sh \
            --network usb0 \
            --flash-only
        ;;

    *)
        echo "Unknown boot mode: $boot"
        echo
        echo "Usage:"
        echo "  ./deploy.sh boot=nvme"
        echo "  ./deploy.sh boot=nfs"
        exit 1
        ;;
esac

step_end

echo "========================== DEPLOY FINISHED =========================="

play -nq -t alsa synth 1 sine 300
