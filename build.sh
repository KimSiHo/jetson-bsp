#!/usr/bin/env bash

set -e

SCRIPTS_DIR="$BASE_DIR/scripts"
BUILD_SCRIPTS_DIR="${SCRIPTS_DIR}/build"

echo "[INFO] Start BSP build pipeline"

echo "[STEP 1] Apply patches"
bash "${BUILD_SCRIPTS_DIR}/apply-patches.sh"

echo "[STEP 2] Build kernel"
bash "${BUILD_SCRIPTS_DIR}/build-kernel.sh"

echo "[STEP 3] Build dtb"
bash "${BUILD_SCRIPTS_DIR}/build-dtb.sh"

echo "[STEP 4] insall package"
bash "${BUILD_SCRIPTS_DIR}/install_package.sh"

echo "[STEP 5] Apply rootfs overlay"
bash "${BUILD_SCRIPTS_DIR}/apply-rootfs-overlay.sh"

echo "[INFO] BSP build pipeline completed successfully"
