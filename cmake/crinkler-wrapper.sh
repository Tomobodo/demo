#!/bin/bash
# Wrapper script invoked by cmake -E vs_link_exe in place of lld-link.
# Filters MSVC/lld-only flags and forwards the rest to Crinkler via Wine.

CRINKLER="${CRINKLER_EXE:-$HOME/tools/Crinkler.exe}"

args=()
skip_next=0

exec wine "$CRINKLER" "$@"
