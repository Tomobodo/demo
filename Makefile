WIDTH = 854
HEIGHT =  480
TARGET_BASE = demo

PLATFORM ?= win32
CONFIG ?= Release
OUTDIR = build/$(CONFIG)_${PLATFORM}

TARGET = $(OUTDIR)/$(TARGET_BASE).exe
OBJDIR = $(OUTDIR)/obj

DEFINES = -DFULLSCREEN -DBUFFER_WIDTH=$(WIDTH) -DBUFFER_HEIGHT=$(HEIGHT) -DSHOW_FPS -DPLATFORM=$(PLATFORM)

ifeq ($(PLATFORM),win32)
	include makefiles/win32.mk
else ifeq ($(PLATFORM),wasm32)
	include makefiles/wasm32.mk
endif

.PHONY: all clean debug_win32 release_win32 debug_wasm release_wasm

all: $(TARGET)

debug_win32:
	$(MAKE) PLATFORM=win32 CONFIG=Debug all

release_win32:
	$(MAKE) PLATFORM=win32 CONFIG=Release all

debug_wasm:
	$(MAKE) PLATFORM=wasm32 CONFIG=Debug all

release_wasm:
	$(MAKE) PLATFORM=wasm32 CONFIG=Release all

$(OBJDIR):
	mkdir -p "$(OBJDIR)"

clean:
	rm -rf "$(OBJDIR)" "$(TARGET)"