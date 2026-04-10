#!/usr/bin/env bash

set -e

SCRIPTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

export VERSION_DIR="${SCRIPTS_DIR}/versions"

mkdir -p "${VERSION_DIR}"

echo "[INFO] Updating apt package index"
sudo apt update

echo "[STEP 1] Install Netlink development packages"
bash "${SCRIPTS_DIR}/modules/setup-netlink.sh"

echo "[STEP 2] Install DeepStream development packages"
bash "${SCRIPTS_DIR}/modules/setup-deepstream-dev.sh"

echo "[INFO] Development environment setup completed successfully"
