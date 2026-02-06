CC = C:/msys64/ucrt64/bin/clang++.exe
SRCS = src/main_wasm.cpp

OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.obj,$(SRCS))

CFLAGS = -O3 -Wall -std=c++latest
LDFLAGS =

$(OBJDIR)/%.obj: src/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) $(DEFINES) -c "$<" -o "$@"

$(TARGET): $(OBJS)
	mkdir -p "$(OUTDIR)"
	$(CC) $(OBJS) -o "$(TARGET)" $(LDFLAGS)
