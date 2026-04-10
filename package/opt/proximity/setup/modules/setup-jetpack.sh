#!/usr/bin/env bash

set -e

VERSION_FILE="${VERSION_DIR}/jetpack-packages.txt"

PACKAGES=(
    nvidia-jetpack
)

echo "[INFO] Installing JetPack"

sudo apt install -y "${PACKAGES[@]}"

echo "[INFO] Recording JetPack version"

dpkg-query -W \
    "${PACKAGES[@]}" \
    > "${VERSION_FILE}"

echo "[INFO] JetPack version saved to ${VERSION_FILE}"

echo "[INFO] JetPack installation completed"