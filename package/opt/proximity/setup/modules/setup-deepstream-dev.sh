#!/usr/bin/env bash

set -e

VERSION_FILE="${VERSION_DIR}/deepstream-dev-packages.txt"

PACKAGES=(
    libssl-dev
    libyaml-cpp-dev
    libgstreamer1.0-dev
    libgstreamer-plugins-base1.0-dev
    libjson-glib-dev
    libgstrtspserver-1.0-dev
)

echo "[INFO] Installing DeepStream development packages"

sudo apt install -y "${PACKAGES[@]}"

echo "[INFO] Recording package versions"

dpkg-query -W \
    "${PACKAGES[@]}" \
    > "${VERSION_FILE}"

echo "[INFO] Package versions saved to ${VERSION_FILE}"

echo "[INFO] DeepStream development packages installation completed"