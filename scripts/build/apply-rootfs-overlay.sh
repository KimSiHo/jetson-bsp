#!/usr/bin/env bash

set -e

ROOTFS_DIR="${L4T_DIR}/rootfs"
OVERLAY_DIR="${BASE_DIR}/rootfs-overlay"

echo "[INFO] Apply rootfs overlay"
echo " overlay : ${OVERLAY_DIR}"
echo " target : ${ROOTFS_DIR}"

if [ ! -d "${OVERLAY_DIR}" ]; then
echo "[ERROR] rootfs-overlay not found"
exit 1
fi

if [ ! -d "${ROOTFS_DIR}" ]; then
echo "[ERROR] Linux_for_Tegra/rootfs not found"
exit 1
fi

sudo rsync -avh --chown=root:root "${OVERLAY_DIR}/" "${ROOTFS_DIR}/"

echo "[INFO] Rootfs overlay applied successfully"
