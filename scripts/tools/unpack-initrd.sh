#!/bin/bash
set -e

# 경로 설정 (스크립트 위치 기준 역산 또는 절대 경로)
INITRD_IMG="$L4T_DIR/bootloader/l4t_initrd.img"
UNPACK_DIR="$L4T_DIR/bootloader/initrd-debug-workspace"

echo "=== [1] 임시 디렉토리 초기화 ==="
if [ -d "$UNPACK_DIR" ]; then
    sudo rm -rf "$UNPACK_DIR"
fi
mkdir -p "$UNPACK_DIR"
cd "$UNPACK_DIR"

echo "=== [2] l4t_initrd.img 압축 해제 ==="
if [ ! -f "$INITRD_IMG" ]; then
    echo "오류: $INITRD_IMG 파일이 존재하지 않습니다."
    exit 1
fi
gzip -dc "$INITRD_IMG" | cpio -idmv > /dev/null 2>&1

echo "=== [3] 포함된 커널 모듈 버전 검증 ==="
# 내부에 풀린 모듈 중 하나를 골라 커널 빌드 정보를 출력
TARGET_KO=$(find . -name "pcie-tegra194.ko" | head -n 1)

if [ -n "$TARGET_KO" ]; then
    echo "정상 확인! initrd 내부 모듈 정보:"
    echo "----------------------------------------"
    modprobe --dump-modversions "$TARGET_KO" | grep "module_layout"
    echo "----------------------------------------"
    echo "작업 공간: $UNPACK_DIR"
else
    echo "경고: PCIe/NVMe 핵심 모듈을 찾을 수 없습니다. 패킹이 잘못되었을 수 있습니다."
fi
