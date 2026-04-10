#!/usr/bin/env bash

set -e

SCRIPTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

export VERSION_DIR="${SCRIPTS_DIR}/versions"

mkdir -p "${VERSION_DIR}"

echo "[INFO] Updating apt package index"
sudo apt update

echo "[STEP 1] Install JetPack"
bash "${SCRIPTS_DIR}/modules/setup-jetpack.sh"

echo "[STEP 2] Install Netlink packages"
bash "${SCRIPTS_DIR}/modules/setup-netlink.sh"

echo "[STEP 3] Install DeepStream packages"
bash "${SCRIPTS_DIR}/modules/setup-deepstream.sh"

echo "[STEP 4] Install utils packages"
bash "${SCRIPTS_DIR}/modules/setup-utils.sh"

echo "[INFO] Target setup completed successfully"
