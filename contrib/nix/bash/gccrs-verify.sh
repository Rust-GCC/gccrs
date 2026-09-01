#!/usr/bin/env bash

ROOT_DIR=$(git rev-parse --show-toplevel 2>/dev/null || true)

if [ -z "$ROOT_DIR" ]; then
    echo "Warning: You must be inside a git repository to run this command." >&2
    exit 1
fi

SCRIPT_PATH="$ROOT_DIR/contrib/gcc-changelog/git_check_commit.py"

if [ ! -f "$SCRIPT_PATH" ]; then
    echo "Warning: '$SCRIPT_PATH' not found! Are you sure this is the gccrs repository?" >&2
    exit 1
fi

cd "$ROOT_DIR"

TARGET_COMMIT="${1:-HEAD}"
python3 "$SCRIPT_PATH" "$TARGET_COMMIT"
