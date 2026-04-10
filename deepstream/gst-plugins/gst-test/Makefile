SYSROOT = $(BASE_DIR)/Linux_for_Tegra/rootfs
PLUGIN_DIR = $(BASE_DIR)/rootfs-overlay/usr/lib/aarch64-linux-gnu/gstreamer-1.0
TARGET = $(OUT_DIR)/libgsttest.so
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

CPPFLAGS = $(GST_CFLAGS)

CXXFLAGS = \
	-Wall \
	-O2 \
	-fPIC

LDFLAGS = \
	-shared \
	-L$(SYSROOT)/usr/lib/aarch64-linux-gnu \
	-L$(SYSROOT)/lib/aarch64-linux-gnu \
	-Wl,-rpath-link,$(SYSROOT)/usr/lib/aarch64-linux-gnu \
	-Wl,-rpath-link,$(SYSROOT)/lib/aarch64-linux-gnu

LDLIBS = $(GST_LIBS)

SRCS = \
	main.cpp \
	gsthello.cpp

.PHONY: all install clean

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(SYSROOT_FLAGS) $(CPPFLAGS) $(CXXFLAGS) $(SRCS) -o $@ $(LDFLAGS) $(LDLIBS)

install: all
	mkdir -p $(PLUGIN_DIR)
	install -m 644 $(TARGET) $(PLUGIN_DIR)

clean:
	rm -rf $(OUT_DIR)
