#!/usr/bin/env bash

set -euo pipefail

SRC_PATH="${L4T_DIR}/kernel/kernel_supplements.tbz2"
DST_DIR="${BASE_DIR}/debug/tbz2"

mkdir -p "${DST_DIR}"

extract_tbz2()
{
    local tbz2_file="$1"

    local pkg_name
    pkg_name="$(basename "${tbz2_file}" .tbz2)"

    local out_dir="${DST_DIR}/${pkg_name}"

    echo "[INFO] ${tbz2_file}"
    echo "       -> ${out_dir}"

    rm -rf "${out_dir}"
    mkdir -p "${out_dir}"

    tar -xjf "${tbz2_file}" -C "${out_dir}"
}

if [[ -d "${SRC_PATH}" ]]; then
    find "${SRC_PATH}" \
        -maxdepth 1 \
        -type f \
        -name "*.tbz2" \
        | sort \
        | while read -r tbz2; do
            extract_tbz2 "${tbz2}"
        done

elif [[ -f "${SRC_PATH}" ]]; then
    extract_tbz2 "${SRC_PATH}"

else
    echo "ERROR: ${SRC_PATH} not found"
    exit 1
fi

echo "[INFO] Done"
