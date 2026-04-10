# jetson-bsp

Jetson Linux BSP를 빌드하고 배포.

NVIDIA에서 제공하는 **Linux_for_Tegra(L4T)** 를 기반으로 커널, 디바이스 트리, 카메라 드라이버, DeepStream 애플리케이션을 개발하며,
빌드 및 배포 환경을 함께 관리합니다.

NVIDIA에서 제공하는 BSP 자체는 버전 관리하지 않고, **스크립트, 패치 및 사용자 개발 코드**를 중심으로 관리합니다.

## 디렉토리 구조

```text
jetson-bsp/
├── Linux_for_Tegra/   # NVIDIA Jetson BSP
├── deepstream/        # DeepStream 애플리케이션 및 공통 코드
├── patches/           # 커널/드라이버/DT 패치
├── scripts/           # 빌드, 배포, Sysroot 구성 자동화
├── rootfs-overlay/    # 타겟 RootFS Overlay
├── sysroot/           # Cross Compile용 Sysroot
├── package/           # 배포 패키지
├── src/               # rootfs 배포 용 사용자 코드
└── README.md
```

## 주요 스크립트

### build.sh

Jetson BSP를 빌드하는 스크립트입니다.

주요 작업

- Linux Kernel 빌드
- In-tree Module 설치
- Kernel Image 복사
- Out-of-tree Module 빌드 및 설치
- `depmod` 실행

### deploy.sh

빌드 결과를 Jetson 보드에 배포하는 스크립트입니다.

필요한 이미지와 파일을 구성한 뒤 NVIDIA Flash 도구를 이용하여 타겟에 배포합니다.

### setup-env

빌드에 필요한 환경 변수를 설정합니다.

### setup-l4t.sh

NVIDIA Linux_for_Tegra 환경을 초기화하고 작업 환경을 구성합니다.

## 개발 방식

- Linux Kernel 및 Device Tree는 Patch 기반으로 관리합니다.
- DeepStream 애플리케이션은 BSP 환경과 함께 개발 및 배포합니다.
- Cross Compile을 위한 Sysroot를 유지하여 Host에서 빌드합니다.
- RootFS Overlay를 이용하여 사용자 애플리케이션과 설정을 관리합니다.

## Dependency

- JetPack: 6.2.1
- L4T: R36.4.4
- Kernel: 5.15.148-tegra
- Board: Jetson Orin Nano Devkit
- Root Device: NVMe (nvme0n1p1)
- Toolchain: aarch64-buildroot-linux-gnu (2022.08)

- GStreamer    : 1.20.3
- CUDA         : 12.6
