SYSROOT = $(BASE_DIR)/Linux_for_Tegra/rootfs
TARGET_DIR = $(BASE_DIR)/rootfs-overlay/usr/local/bin
TARGET = $(OUT_DIR)/app
OUT_DIR = ../../out

PKG_CONFIG = $(SCRIPTS_DIR)/wrappers/aarch64-pkg-config
GST_PKG = gstreamer-1.0

GST_CFLAGS = $(shell $(PKG_CONFIG) --cflags $(GST_PKG))
GST_LIBS   = $(shell $(PKG_CONFIG) --libs $(GST_PKG))

ifeq ($(VERBOSE),1)
$(info GST_CFLAGS = $(GST_CFLAGS))
$(info GST_LIBS   = $(GST_LIBS))
endif

CXX = $(CROSS_COMPILE)g++

SYSROOT_FLAGS = --sysroot=$(SYSROOT)

CPPFLAGS = \
	-I$(SYSROOT)/opt/nvidia/deepstream/deepstream-7.1/sources/includes \
	$(GST_CFLAGS)

CXXFLAGS = -Wall -O2

LDFLAGS = \
	-L$(SYSROOT)/usr/lib/aarch64-linux-gnu \
	-L$(SYSROOT)/lib/aarch64-linux-gnu \
	-L$(SYSROOT)/opt/nvidia/deepstream/deepstream-7.1/lib \
	-Wl,-rpath-link,$(SYSROOT)/usr/lib/aarch64-linux-gnu \
	-Wl,-rpath-link,$(SYSROOT)/lib/aarch64-linux-gnu

LDLIBS = \
	$(GST_LIBS) \
	-lnvdsgst_meta \
	-lnvds_meta

.PHONY: all install clean

all: $(TARGET)

$(TARGET): main.cpp
	@mkdir -p $(OUT_DIR)
	$(CXX) $(SYSROOT_FLAGS) $(CPPFLAGS) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

install: all
	mkdir -p $(TARGET_DIR)
	cp $(TARGET) $(TARGET_DIR)/

clean:
	rm -rf $(OUT_DIR)
