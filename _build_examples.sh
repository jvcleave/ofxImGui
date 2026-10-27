#!/usr/bin/env bash
# Builds every example with make: Linux, macOS, MSYS2 (UCRT64) or Emscripten.
#
#   ./_build_examples.sh [Debug|Release]       OF_ROOT defaults to ../.. (addons/ofxImGui)
#   emmake ./_build_examples.sh                Emscripten: emsdk in PATH
#   EXAMPLES="example-simple example-demo" ./_build_examples.sh
#
# example-ios needs Xcode; example-ImGuizmo is skipped without addons/ofxImGuizmo.
set -uo pipefail

ADDON_DIR=$(cd "$(dirname "$0")" && pwd)
OF_ROOT=${OF_ROOT:-$(cd "$ADDON_DIR/../.." && pwd)}
TARGET=${1:-Debug}
JOBS=$(getconf _NPROCESSORS_ONLN 2> /dev/null || echo 4)
MAKE_ARGS=(OF_ROOT="$OF_ROOT" -j"$JOBS")

if [[ -n ${EMSDK:-} ]]; then
	PLATFORM=emscripten
elif [[ -n ${MSYSTEM:-} ]]; then
	PLATFORM=msys2
	MAKE_ARGS+=(MINGW_PREFIX="${MINGW_PREFIX:-/ucrt64}")
else
	case $(uname -s) in
		Darwin) PLATFORM=osx ;;
		*) PLATFORM=linux ;;
	esac
fi

PASS=()
FAIL=()
SKIP=()
if [[ -n ${EXAMPLES:-} ]]; then
	read -ra names <<< "$EXAMPLES"
else
	names=()
	for dir in "$ADDON_DIR"/example-*/; do names+=("$(basename "$dir")"); done
fi
for name in "${names[@]}"; do
	example_dir=$ADDON_DIR/$name
	if [[ $name == example-ios ]] || { [[ $name == example-ImGuizmo ]] && [[ ! -d $OF_ROOT/addons/ofxImGuizmo ]]; }; then
		SKIP+=("$name")
		continue
	fi
	echo "====== $name ($PLATFORM $TARGET) ======"
	cp "$OF_ROOT/scripts/templates/$PLATFORM/Makefile" "$OF_ROOT/scripts/templates/$PLATFORM/config.make" "$example_dir/"
	if make -C "$example_dir" "${MAKE_ARGS[@]}" "$TARGET"; then
		PASS+=("$name")
	else
		FAIL+=("$name")
	fi
done

echo
echo "PASSED (${#PASS[@]}): ${PASS[*]:-none}"
echo "FAILED (${#FAIL[@]}): ${FAIL[*]:-none}"
echo "SKIPPED (${#SKIP[@]}): ${SKIP[*]:-none}"
[[ ${#FAIL[@]} -eq 0 ]]
