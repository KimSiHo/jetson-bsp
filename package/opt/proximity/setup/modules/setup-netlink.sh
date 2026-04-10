#!/usr/bin/env bash

set -e

VERSION_FILE="${VERSION_DIR}/netlink-packages.txt"

PACKAGES=(
    libnl-3-dev
    libnl-genl-3-dev
)

echo "[INFO] Installing Netlink packages"

sudo apt install -y "${PACKAGES[@]}"

echo "[INFO] Recording Netlink package versions"

dpkg-query -W \
    "${PACKAGES[@]}" \
    > "${VERSION_FILE}"

echo "[INFO] Netlink package versions saved to ${VERSION_FILE}"

echo "[INFO] Netlink package installation completed"