#!/usr/bin/env bash
set -euo pipefail
export MSYSTEM=UCRT64
export CHERE_INVOKING=1
export PATH="/ucrt64/bin:/usr/bin:/bin:/usr/local/bin${PATH+:$PATH}"

OF_ROOT='/d/repos/openFrameworks'
ADDON_DIR="$OF_ROOT/addons/ofxImGui"
MAKE_ARGS="OF_ROOT=$OF_ROOT MINGW_PREFIX=/ucrt64 -j8"
PASS=()
FAIL=()

for example_dir in "$ADDON_DIR"/example-*/; do
    name=$(basename "$example_dir")
    # Skip ios (cross-compile only)
    if [[ "$name" == "example-ios" ]]; then
        echo "====== SKIP: $name ======"
        continue
    fi
    echo ""
    echo "====== Building: $name ======"
    if make -C "$example_dir" $MAKE_ARGS Debug 2>&1; then
        PASS+=("$name")
        echo "[OK] $name"
    else
        ret=$?
        FAIL+=("$name")
        echo "[FAIL] $name (exit $ret)"
    fi
done

echo ""
echo "=============================="
echo "SUMMARY"
echo "=============================="
echo "PASSED (${#PASS[@]}): ${PASS[*]:-none}"
echo "FAILED (${#FAIL[@]}): ${FAIL[*]:-none}"
[ ${#FAIL[@]} -eq 0 ] && exit 0 || exit 1
