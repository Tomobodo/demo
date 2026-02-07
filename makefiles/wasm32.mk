CC = C:/msys64/ucrt64/bin/clang++.exe

INCLUDES= \
	-I"src/common"

CFLAGS = $(INCLUDES) -O3 -std=c++23 -fms-extensions
LDFLAGS =

$(OBJDIR)/%.obj: %.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEFINES) -c "$<" -o "$@"

$(TARGET): $(OBJS)
	mkdir -p "$(OUTDIR)"
	$(CC) $(OBJS) -o "$(TARGET)" $(LDFLAGS)
