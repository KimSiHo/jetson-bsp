SYSROOT = $(BASE_DIR)/Linux_for_Tegra/rootfs
TARGET_DIR = $(BASE_DIR)/rootfs-overlay/usr/local/bin
OUT_DIR ?= ../out

CC = $(CROSS_COMPILE)gcc
CFLAGS := -I$(SYSROOT)/usr/include -I$(SYSROOT)/usr/include/libnl3 -Wall -O2
LDFLAGS := -L$(SYSROOT)/usr/lib/aarch64-linux-gnu -L$(SYSROOT)/lib/aarch64-linux-gnu -lnl-3 -lnl-genl-3

TARGET = $(OUT_DIR)/my_genl_app
OBJS = $(OUT_DIR)/main.o

.PHONY: all install
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJS): main.c
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	mkdir -p $(TARGET_DIR)
	cp $(TARGET) $(TARGET_DIR)/
