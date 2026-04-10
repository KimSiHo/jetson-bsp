#!/usr/bin/env bash

set -e

ROOTFS_TBZ2="${L4T_DIR}/Tegra_Linux_Sample-Root-Filesystem_R36.4.4_aarch64.tbz2"
ROOTFS_DIR="${L4T_DIR}/rootfs"

echo "[INFO] Setup rootfs"

if [ ! -f "${ROOTFS_TBZ2}" ]; then
    echo "[ERROR] Sample rootfs not found:"
    echo "        ${ROOTFS_TBZ2}"
    exit 1
fi

echo "[INFO] Cleaning rootfs..."
sudo rm -rf "${ROOTFS_DIR:?}"/*

echo "[INFO] Extracting sample rootfs..."
sudo tar xpf "${ROOTFS_TBZ2}" -C "${ROOTFS_DIR}"

echo "[INFO] Applying NVIDIA binaries..."
(
    cd "${L4T_DIR}"
    sudo ./apply_binaries.sh
)

echo "[INFO] Rootfs setup completed"
