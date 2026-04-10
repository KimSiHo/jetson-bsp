#!/bin/bash

set -e

PACKAGE_DIR="${BASE_DIR}/package"
OVERLAY_DIR="${BASE_DIR}/rootfs-overlay"

echo "Installing package..."

if [ -d "${PACKAGE_DIR}/boot" ]; then
    rsync -a --delete \
        "${PACKAGE_DIR}/boot/" \
        "${OVERLAY_DIR}/boot/"
fi

if [ -d "${PACKAGE_DIR}/etc" ]; then
    rsync -a --delete \
        "${PACKAGE_DIR}/etc/" \
        "${OVERLAY_DIR}/etc/"
fi

if [ -d "${PACKAGE_DIR}/usr" ]; then
    rsync -a --delete \
        "${PACKAGE_DIR}/usr/" \
        "${OVERLAY_DIR}/usr/"
fi

if [ -d "${PACKAGE_DIR}/opt" ]; then
    rsync -a --delete \
        "${PACKAGE_DIR}/opt/" \
        "${OVERLAY_DIR}/opt/"
fi

echo "Done."
