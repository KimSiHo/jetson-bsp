#!/bin/bash

set -e

PATCH_DIR=$BASE_DIR/patches

apply_series() {
    TARGET_DIR=$1
    SERIES_FILE=$2
    PATCH_BASE=$(dirname "$SERIES_FILE")
    echo "PATCH_BASE: $PATCH_BASE"

    if [ ! -f "$SERIES_FILE" ]; then
        return
    fi

    echo "Applying patches in $SERIES_FILE"

    echo "cd -> $TARGET_DIR"
    cd "$TARGET_DIR"

  while read -r patch; do
      [[ -z "$patch" || "$patch" =~ ^# ]] && continue

      PATCH_PATH="$PATCH_BASE/$patch"

      echo "  -> $patch"

      if git am --3way "$PATCH_PATH"; then
          continue
      fi

      echo "Patch failed. checking already applied..."

      git am --abort

      if git apply --reverse --check "$PATCH_PATH" >/dev/null 2>&1; then
          echo "Already applied. skipping..."
          continue
      fi

      echo "Real conflict detected."
      exit 1

  done < "$SERIES_FILE"
}

echo "======================= APPLY PATCHES ======================="

echo "======================= KERNEL PATCHES ======================="
apply_series \
"$L4T_DIR/source/kernel/kernel-jammy-src" \
"$PATCH_DIR/kernel-jammy-src/$PHASE/series"

echo "======================= NVIDIA-OOT PATCHES ======================="
apply_series \
"$L4T_DIR/source/nvidia-oot" \
"$PATCH_DIR/nvidia-oot/$PHASE/series"

echo "======================= NVDISPLAY PATCHES ======================="
apply_series \
"$L4T_DIR/source/nvdisplay" \
"$PATCH_DIR/nvdisplay/$PHASE/series"

echo "======================= KERNEL-DEVICETREE PATCHES ======================="
apply_series \
"$L4T_DIR/source/hardware/nvidia/t23x/nv-public" \
"$PATCH_DIR/kernel-devicetree/$PHASE/series"

echo "======================= PATCH APPLY DONE ======================="
