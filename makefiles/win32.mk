SDK_VERSION = 10.0.26100.0
MSVC_VERSION = 14.50.35717

VS_DIR = C:/Program Files/Microsoft Visual Studio/18/Community
KITS_DIR = C:/Program Files (x86)/Windows Kits/10

CC = "$(VS_DIR)/VC/Tools/MSVC/$(MSVC_VERSION)/bin/Hostx64/x86/cl.exe"

LINK_MS = "$(VS_DIR)/VC/Tools/MSVC/$(MSVC_VERSION)/bin/Hostx64/x86/link.exe"
LINK_CR = Crinkler.exe

INCLUDES = \
	-I"src/common" \
	-I"$(KITS_DIR)/Include/$(SDK_VERSION)/um" \
	-I"$(KITS_DIR)/Include/$(SDK_VERSION)/shared" \
	-I"$(KITS_DIR)/Include/$(SDK_VERSION)/ucrt" \
	-I"$(VS_DIR)/VC/Tools/MSVC/$(MSVC_VERSION)/include"

LIB_SDK = "$(KITS_DIR)/Lib/$(SDK_VERSION)/um/x86"
LIB_CRT = "$(KITS_DIR)/Lib/$(SDK_VERSION)/ucrt/x86"
LIB_MSVC = "$(VS_DIR)/VC/Tools/MSVC/$(MSVC_VERSION)/lib/x86"

LIBPATHS = \
	/LIBPATH:$(LIB_SDK) \
	/LIBPATH:$(LIB_CRT) \
	/LIBPATH:$(LIB_MSVC)

LIBS = kernel32.lib user32.lib gdi32.lib dwmapi.lib

ifeq ($(CONFIG),Debug)
	LINK = $(LINK_MS)

	CFLAGS_CFG = -Od -Z7 -GS- -DDEBUG
	LDFLAGS_CFG = \
		$(LIBPATHS) \
		//NODEFAULTLIB \
		//DEBUG \
		//INCREMENTAL:NO \
		//SUBSYSTEM:WINDOWS \
		//ENTRY:entry

else ifeq ($(CONFIG),Release)
	LINK = $(LINK_CR)

	CFLAGS_CFG = -Os -GS- -EHa- -Zl -Gw -Gy

	LDFLAGS_CFG = \
		$(LIBPATHS) \
		//NODEFAULTLIB \
		//ENTRY:entry \
		//SUBSYSTEM:WINDOWS \
		//TINYHEADER \
		//TINYIMPORT \
		//COMPMODE:VERYSLOW \
		//ORDERTRIES:1000
else
	$(error CONFIG must be Debug or Release)
endif

CFLAGS = -nologo -c $(INCLUDES) $(CFLAGS_CFG)

$(OBJDIR)/%.obj: %.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEFINES) -Oi -std:c++latest -GR- -Fo"$@" "$<"

$(TARGET): $(OBJS)
	mkdir -p "$(OUTDIR)"
	$(LINK) $(LDFLAGS_CFG) $(OBJS) $(LIBS)  /OUT:"$(TARGET)"