#!/usr/bin/env bash

set -e

DEEPSTREAM_DEB="deepstream-7.1_7.1.0-1_arm64.deb"
DEEPSTREAM_URL="https://api.ngc.nvidia.com/v2/resources/nvidia/deepstream/versions/7.1/files/${DEEPSTREAM_DEB}"

VERSION_FILE="${VERSION_DIR}/deepstream-packages.txt"

PACKAGES=(
    libssl3
    libgstreamer1.0-0
    gstreamer1.0-tools
    gstreamer1.0-plugins-good
    gstreamer1.0-plugins-bad
    gstreamer1.0-plugins-ugly
    gstreamer1.0-libav
    libgstrtspserver-1.0-0
    libjansson4
)

echo "[INFO] Installing DeepStream dependencies"

sudo apt install -y "${PACKAGES[@]}"

echo "[INFO] Downloading DeepStream"

cd /tmp

if ! dpkg -s deepstream-7.1 >/dev/null 2>&1; then
    if [ ! -f "${DEEPSTREAM_DEB}" ]; then
        wget -O "${DEEPSTREAM_DEB}" "${DEEPSTREAM_URL}"
    fi

    echo "[INFO] Installing DeepStream"

    sudo dpkg -i "${DEEPSTREAM_DEB}"
fi

echo "[INFO] Recording package versions"

dpkg-query -W \
    deepstream-7.1 \
    "${PACKAGES[@]}" \
    > "${VERSION_FILE}"

echo "[INFO] Package versions saved to ${VERSION_FILE}"

echo "[INFO] DeepStream installation completed"
