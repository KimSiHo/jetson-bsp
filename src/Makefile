MODULE  = thermal gst
OUT_DIR = out

all:
	mkdir -p $(OUT_DIR)
	$(MAKE) -C $(MODULE) OUT_DIR=$(CURDIR)/$(OUT_DIR)

clean:
	rm -rf $(OUT_DIR)

install: all
	$(MAKE) -C $(MODULE) install OUT_DIR=$(CURDIR)/$(OUT_DIR)