#!/bin/bash
# Wrapper script invoked by cmake -E vs_link_exe in place of lld-link.
# Filters MSVC/lld-only flags and forwards the rest to Crinkler via Wine.

CRINKLER="${CRINKLER_EXE:-$HOME/tools/Crinkler.exe}"

args=()
skip_next=0

for arg in "$@"; do
    if [[ $skip_next -eq 1 ]]; then
        skip_next=0
        continue
    fi

    # Strip flags Crinkler does not understand
    case "$arg" in
        /nologo)             continue ;;
        /MANIFEST:*)         continue ;;
        /MANIFESTUAC:*)      continue ;;
        /manifestdependency:*) continue ;;
        /implib:*)           continue ;;
        /pdb:*)              continue ;;
        /version:*)          continue ;;
        /machine:*)          continue ;;
        /INCREMENTAL*)       continue ;;
        /DEBUG*)             continue ;;
        /LTCG)               continue ;;   # bare /LTCG (not /LTCG:NOSTATUS)
        /TINYHEADER)         continue ;;   # 32-bit only
        /TINYIMPORT)         continue ;;   # 32-bit only
        /PROGRESSGUI)        continue ;;   # GUI dialog, use headless mode
        /LARGEADDRESSAWARE:NO) continue ;; # 32-bit only
        *)
            args+=("$arg")
            ;;
    esac
done

exec wine "$CRINKLER" "${args[@]}"
