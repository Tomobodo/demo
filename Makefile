WIDTH = 854
HEIGHT =  480
TARGET_BASE = demo

PLATFORM ?= win32
CONFIG ?= Release
OUTDIR = build/$(CONFIG)_${PLATFORM}

TARGET = $(OUTDIR)/$(TARGET_BASE).exe
OBJDIR = $(OUTDIR)/obj

DEFINES = -DFULLSCREEN -DBUFFER_WIDTH=$(WIDTH) -DBUFFER_HEIGHT=$(HEIGHT) -DSHOW_FPS -DPLATFORM_$(PLATFORM)

SRCS = $(wildcard src/$(PLATFORM)/*.cpp) $(wildcard src/common/*.cpp)
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.obj,$(notdir $(SRCS)))
vpath %.cpp src/common src/$(PLATFORM)

ifeq ($(PLATFORM),win32)
	include makefiles/win32.mk
else ifeq ($(PLATFORM),wasm32)
	include makefiles/wasm32.mk
endif

.PHONY: all clean everything clean_everything

all: $(TARGET)

everything:
	$(MAKE) PLATFORM=win32 CONFIG=Debug all
	$(MAKE) PLATFORM=wasm32 CONFIG=Debug all

$(OBJDIR):
	mkdir -p "$(OBJDIR)"

clean:
	rm -rf "$(OBJDIR)" "$(TARGET)"

clean_everything:
	rm -rf "build/"