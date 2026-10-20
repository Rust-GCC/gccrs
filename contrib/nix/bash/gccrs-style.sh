#!/usr/bin/env bash

ROOT_DIR=$(git rev-parse --show-toplevel 2>/dev/null)

if [ -z "$ROOT_DIR" ]; then
    echo "Warning: You must be inside a git repository to run this command." >&2
    exit 1
fi

SCRIPT_PATH="$ROOT_DIR/contrib/check_GNU_style.py"

if [ ! -f "$SCRIPT_PATH" ]; then
    echo "Warning: '$SCRIPT_PATH' not found! Are you sure this is the gccrs repository?" >&2
    exit 1
fi

arg=""
if [ $# -ge 1 ] && [ "$1" != "-f" ]; then
    arg="$1"
    shift
elif [ $# -eq 3 ]; then
    arg="$3"
    set -- "$1" "$2"
fi

TARGET_COMMIT="${arg:-HEAD}"

git show "$TARGET_COMMIT" | python3 "$SCRIPT_PATH" "$@" -
