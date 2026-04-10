#!/usr/bin/env bash

set -euo pipefail

JETSON_URL="https://developer.nvidia.com/downloads/embedded/l4t/r36_release_v4.4/release/Jetson_Linux_r36.4.4_aarch64.tbz2"

BSP_TARBALL="$(find . -maxdepth 1 -name 'Jetson_Linux*.tbz2' | head -n1)"
ROOTFS_TARBALL="$(find . -maxdepth 1 -name 'Tegra_Linux_Sample-Root-Filesystem*.tbz2' | head -n1)"

if [ -z "${BSP_TARBALL}" ]; then
    read -rp "Jetson BSP not found. Download from NVIDIA? [y/N] " ans

    if [[ "${ans}" =~ ^[Yy]$ ]]; then
        wget "${JETSON_URL}"
        BSP_TARBALL="$(find . -maxdepth 1 -name 'Jetson_Linux*.tbz2' | head -n1)"
    else
        echo "Cancelled."
        exit 1
    fi
fi

echo "Extracting ${BSP_TARBALL} ..."

TMPDIR="$(mktemp -d)"
tar -xf "${BSP_TARBALL}" -C "${TMPDIR}"

cp -a "${TMPDIR}"/Jetson_Linux*/Linux_for_Tegra .

rm -rf "${TMPDIR}"

if [ -z "${ROOTFS_TARBALL}" ]; then
    echo "ERROR: Tegra_Linux_Sample-Root-Filesystem*.tbz2 not found"
    exit 1
fi

echo "[1/5] Extract BSP"
tar xf "${BSP_TARBALL}"

L4T_DIR="Linux_for_Tegra"

if [ ! -d "${L4T_DIR}" ]; then
    echo "ERROR: ${L4T_DIR} not found"
    exit 1
fi

echo "[2/5] Extract Sample RootFS"
sudo tar xpf "${ROOTFS_TARBALL}" \
    -C "${L4T_DIR}/rootfs"

echo "[3/5] Apply NVIDIA binaries"
pushd "${L4T_DIR}" >/dev/null
sudo ./apply_binaries.sh
popd >/dev/null

echo "[4/5] Verify"
test -f "${L4T_DIR}/rootfs/etc/nv_tegra_release"

echo "[5/5] Complete"

echo
echo "Linux_for_Tegra is ready."
echo "Location:"
echo "  ${PWD}/${L4T_DIR}"