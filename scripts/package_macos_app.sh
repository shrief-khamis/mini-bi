#!/usr/bin/env bash
# Copy the built minibi.app into artifacts/ and run macdeployqt on the copy.
# Requires: macOS, a Release (recommended) build that produced minibi.app, Qt kit on disk.
#
# Usage:
#   export QT_PREFIX="/path/to/Qt/6.x.x/macos"   # same root you use for CMAKE_PREFIX_PATH
#   ./scripts/package_macos_app.sh [BUILD_DIR]
#
# BUILD_DIR is relative to repo root, default: build

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${1:-${BUILD_DIR:-build}}"
BUILD_ABS="$ROOT/$BUILD_DIR"

if [[ "$(uname -s)" != "Darwin" ]]; then
  echo "This script is for macOS only." >&2
  exit 1
fi

if [[ -z "${QT_PREFIX:-}" ]]; then
  echo "Set QT_PREFIX to your Qt kit path (e.g. .../Qt/6.8.0/macos), same as CMAKE_PREFIX_PATH." >&2
  exit 1
fi

MACDEPLOYQT="$QT_PREFIX/bin/macdeployqt"
if [[ ! -x "$MACDEPLOYQT" ]]; then
  echo "macdeployqt not found or not executable: $MACDEPLOYQT" >&2
  exit 1
fi

if [[ ! -d "$BUILD_ABS" ]]; then
  echo "Build directory not found: $BUILD_ABS" >&2
  exit 1
fi

# Prefer the newest minibi.app under BUILD_DIR (covers build/, cmake-release/, Release/, etc.)
SRC_APP=""
LATEST=0
while IFS= read -r -d '' app; do
  m="$(stat -f %m "$app" 2>/dev/null || echo 0)"
  if [[ "$m" -gt "$LATEST" ]]; then
    LATEST="$m"
    SRC_APP="$app"
  fi
done < <(find "$BUILD_ABS" -name "minibi.app" -maxdepth 4 -type d -print0 2>/dev/null)

if [[ -z "$SRC_APP" ]]; then
  echo "No minibi.app found under $BUILD_ABS" >&2
  echo "Configure and build for macOS with MACOSX_BUNDLE (Release recommended)." >&2
  exit 1
fi
ARTIFACTS="$ROOT/artifacts"
DEST="$ARTIFACTS/minibi.app"

mkdir -p "$ARTIFACTS"
rm -rf "$DEST"
echo "Copying: $SRC_APP -> $DEST"
cp -R "$SRC_APP" "$DEST"

echo "Running macdeployqt..."
"$MACDEPLOYQT" "$DEST" -verbose=2

echo "Done: $DEST"
du -sh "$DEST"
