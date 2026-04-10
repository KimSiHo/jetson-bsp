#!/usr/bin/env bash

set -e

TARGET_IP="${TARGET_IP:-192.168.45.95}"
SYSROOT_DIR="${BASE_DIR}/sysroot"

mkdir -p "${SYSROOT_DIR}"

echo "=========================================="
echo " Jetson Sysroot rsync"
echo "=========================================="

sudo rsync -avHAX \
    --numeric-ids \
    --delete \
    --delete-excluded \
    --include=/etc/*** \
    --include=/lib/*** \
    --include=/usr/*** \
    --include=/opt/*** \
    --exclude=/usr/lib/modules/*** \
    --exclude='*' \
    "root@${TARGET_IP}:/" \
    "${SYSROOT_DIR}/"

cd "${SYSROOT_DIR}"

echo
echo "=========================================="
echo " Fix absolute symlinks"
echo "=========================================="

fixed=0
skipped=0

while IFS= read -r link; do
    target=$(sudo readlink "${link}")

    # Only process absolute symlinks
    if [[ "${target}" == /* ]]; then
        target_in_sysroot=".${target}"

        if [[ -e "${target_in_sysroot}" || -L "${target_in_sysroot}" ]]; then
            link_dir=$(dirname "${link}")
            rel_target=$(realpath --relative-to="${link_dir}" "${target_in_sysroot}")

            echo "Fixing: ${link}"
            echo "  Old: ${target}"
            echo "  New: ${rel_target}"

            sudo rm -f "${link}"
            sudo ln -s "${rel_target}" "${link}"

            ((fixed++))
        else
            echo "SKIP (missing): ${link} -> ${target}"
            ((skipped++))
        fi
    fi
done < <(sudo find . -type l)

echo
echo "Fixed  : ${fixed} symlinks"
echo "Skipped: ${skipped} symlinks"
