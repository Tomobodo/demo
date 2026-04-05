#!/bin/bash
# Wrapper invoked by CMake in place of the compiler driver when USE_SMOL=ON.
# Translates a gcc-style link invocation to smold.py arguments.

SMOL_DIR="${SMOL_DIR:-$HOME/tools/smol}"
SMOLD="$SMOL_DIR/smold.py"

output=""
objects=()
libs=()
libdirs=()

i=1
while [ $i -le $# ]; do
    arg="${!i}"

    case "$arg" in
        -o)
            i=$((i+1))
            output="${!i}"
            ;;
        *.o)
            objects+=("$arg")
            ;;
        *.a)
            objects+=("$arg")
            ;;
        -l*)
            libs+=("$arg")
            ;;
        -L*)
            libdirs+=("$arg")
            ;;
        *".so" | *".so."*)
            # Full path to shared library — extract dir and name
            dir=$(dirname "$arg")
            base=$(basename "$arg")
            name=$(echo "$base" | sed 's/^lib//;s/\.so.*$//')
            libdirs+=("-L$dir")
            libs+=("-l$name")
            ;;
        *)
            # Skip compiler/linker flags incompatible with smold.py
            ;;
    esac

    i=$((i+1))
done

if [ -z "$output" ]; then
    echo "smol-wrapper: error: no output file specified" >&2
    exit 1
fi

exec python3 "$SMOLD" -fuse-dt-debug -fno-ifunc-support \
    --smolrt "$SMOL_DIR/rt" --smolld "$SMOL_DIR/ld" \
    --cc "$(which gcc)" \
    "${libdirs[@]}" "${libs[@]}" "${objects[@]}" "$output"
