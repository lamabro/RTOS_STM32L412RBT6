#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "Build script running from: $SCRIPT_DIR"

if [[ -z "${CUBEIDE_PATH:-}" ]]; then
  if command -v stm32cubeide >/dev/null 2>&1; then
    CUBEIDE_PATH="$(command -v stm32cubeide)"
  fi
fi

if [[ -n "${CUBEIDE_PATH:-}" ]]; then
  echo "Found STM32CubeIDE at: $CUBEIDE_PATH"
  mapfile -d '' projects < <(find "$SCRIPT_DIR" -name .project -print0)
  if [[ ${#projects[@]} -gt 0 ]]; then
    for projfile in "${projects[@]}"; do
      projname="$(sed -n 's:.*<name>\(.*\)</name>.*:\1:p' "$projfile" | head -n 1)"
      if [[ -z "$projname" ]]; then
        echo "Warning: could not parse project name from $projfile"
        continue
      fi
      echo "----------------------------------------"
      echo "Building project: $projname (Debug)"
      "$CUBEIDE_PATH" -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "$SCRIPT_DIR" -build "$projname/Debug"
      echo "Building project: $projname (Release)"
      "$CUBEIDE_PATH" -nosplash -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "$SCRIPT_DIR" -build "$projname/Release"
    done
    echo "STM32CubeIDE headless build finished."
    exit 0
  fi
fi

echo "STM32CubeIDE not available or no Eclipse projects found. Using CMake/Ninja fallback."
if [[ ! -f CMakePresets.json ]]; then
  echo "Error: CMakePresets.json not found in repository root." >&2
  exit 1
fi

echo "Building Debug preset..."
cmake --preset=Debug-Application
ninja -C build/Target/application/Debug

echo "Building Release preset..."
cmake --preset=Release-Application
ninja -C build/Target/application/Release

echo "Build complete."
