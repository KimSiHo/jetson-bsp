#!/usr/bin/env bash

set -euo pipefail

SRC_PATH="${L4T_DIR}/nv_tegra/l4t_deb_packages/nvidia-l4t-apt-source_36.4.4-20250616085344_arm64.deb"
DST_DIR="${BASE_DIR}/debug/debs"

mkdir -p "${DST_DIR}"

extract_deb()
{
    local deb_file="$1"

    local pkg_name
    pkg_name="$(basename "${deb_file}" .deb)"

    local out_dir="${DST_DIR}/${pkg_name}"

    echo "[INFO] ${deb_file}"
    echo "       -> ${out_dir}"

    rm -rf "${out_dir}"
    mkdir -p "${out_dir}"

    dpkg-deb -x "${deb_file}" "${out_dir}"
}

if [[ -d "${SRC_PATH}" ]]; then
    find "${SRC_PATH}" \
        -maxdepth 1 \
        -type f \
        -name "*.deb" \
        | sort \
        | while read -r deb; do
            extract_deb "${deb}"
        done

elif [[ -f "${SRC_PATH}" ]]; then
    extract_deb "${SRC_PATH}"

else
    echo "ERROR: ${SRC_PATH} not found"
    exit 1
fi

echo "[INFO] Done"
