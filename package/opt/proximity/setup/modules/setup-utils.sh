#!/usr/bin/env bash

set -e

VERSION_FILE="${VERSION_DIR}/utils-packages.txt"

PACKAGES=(
    graphviz
)

echo "[INFO] Installing utils packages"

sudo apt install -y "${PACKAGES[@]}"

echo "[INFO] Recording utils package versions"

dpkg-query -W \
    "${PACKAGES[@]}" \
    > "${VERSION_FILE}"

echo "[INFO] Utils package versions saved to ${VERSION_FILE}"

echo "[INFO] Utils package installation completed"