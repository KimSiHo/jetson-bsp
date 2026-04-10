CUDA_VER := 12.6
NVDS_VERSION := 7.1

APP := deepstream-proximity-record

VERBOSE ?= 0
BUILD ?= cross

OUT_DIR := ../out
OBJ_DIR := $(OUT_DIR)/obj
TARGET := $(OUT_DIR)/$(APP)

ifeq ($(BUILD),cross)

APP_INSTALL_DIR := $(BASE_DIR)/rootfs-overlay/opt/deepstream-proximity-record/bin
SAMPLE_INSTALL_DIR := $(BASE_DIR)/rootfs-overlay/opt/deepstream-proximity-record/samples
PKG_CONFIG := $(SCRIPTS_DIR)/wrappers/aarch64-pkg-config

CROSS_COMPILE := /home/sihokim/bin/x-tools/aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
SYSROOT := $(BASE_DIR)/sysroot
SYSROOT_FLAGS := --sysroot=$(SYSROOT)

else

APP_INSTALL_DIR := /opt/deepstream-proximity-record/bin
PKG_CONFIG := pkg-config

CROSS_COMPILE :=
SYSROOT :=
SYSROOT_FLAGS :=

endif

CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++

DS_ROOT := $(SYSROOT)/opt/nvidia/deepstream/deepstream-$(NVDS_VERSION)
CUDA_ROOT := $(SYSROOT)/usr/local/cuda-$(CUDA_VER)
APPS_COMMON := ../apps-common

PKGS := gstreamer-1.0 gstreamer-video-1.0 x11 json-glib-1.0

PKG_CFLAGS := $(shell $(PKG_CONFIG) --cflags $(PKGS))
PKG_LIBS   := $(shell $(PKG_CONFIG) --libs $(PKGS))

CFLAGS += $(PKG_CFLAGS)
LIBS   += $(PKG_LIBS)

ifeq ($(VERBOSE),1)
$(info PKG_CFLAGS = $(PKG_CFLAGS))
$(info PKG_LIBS   = $(PKG_LIBS))
endif

CFLAGS += \
	-Iinclude \
	-Iprotocol/include \
	-I$(APPS_COMMON)/includes \
	-I$(DS_ROOT)/sources/includes \
	-I$(CUDA_ROOT)/include

LDFLAGS += \
	-L$(SYSROOT)/usr/lib/aarch64-linux-gnu/nvidia \
	-L$(DS_ROOT)/lib \
	-L$(CUDA_ROOT)/lib64 \
	-Wl,-rpath-link,$(CUDA_ROOT)/targets/aarch64-linux/lib \
	-Wl,-rpath-link,$(SYSROOT)/lib/aarch64-linux-gnu \
	-Wl,-rpath-link,$(SYSROOT)/usr/lib/aarch64-linux-gnu \
	-Wl,-rpath-link,$(SYSROOT)/usr/lib/aarch64-linux-gnu/nvidia

LIBS += \
	-lcudart \
	-lcuda \
	-lnvdsgst_meta \
	-lnvds_meta \
	-lnvdsgst_helper \
	-lnvdsgst_customhelper \
	-lnvdsgst_smartrecord \
	-lnvds_utils \
	-lnvds_logger \
	-lyaml-cpp \
	-lgstrtspserver-1.0

ifeq ($(VERBOSE),1)
$(info BUILD     = $(BUILD))
$(info SYSROOT   = $(SYSROOT))
$(info CC        = $(CC))
$(info CXX       = $(CXX))
$(info PKG_CONFIG= $(PKG_CONFIG))
$(info CFLAGS    = $(CFLAGS))
$(info LDFLAGS   = $(LDFLAGS))
$(info LIBS      = $(LIBS))
endif

SRCS := $(wildcard src/*.c)
SRCS += $(wildcard src/*.cpp)
SRCS += $(wildcard src/uds/*.c)
SRCS += $(wildcard $(APPS_COMMON)/src/*.c)

OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SRCS)))
OBJS += $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(filter %.cpp,$(SRCS)))

# for debug
CFLAGS += -g3 -O0

.PHONY: all install clean

all: $(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(SYSROOT_FLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(SYSROOT_FLAGS) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(SYSROOT_FLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

install: all
	mkdir -p $(APP_INSTALL_DIR)
	cp $(TARGET) $(APP_INSTALL_DIR)/

ifeq ($(BUILD),cross)
	mkdir -p $(SAMPLE_INSTALL_DIR)

	cp -a samples/. $(SAMPLE_INSTALL_DIR)
endif

clean:
	rm -rf $(OUT_DIR)
