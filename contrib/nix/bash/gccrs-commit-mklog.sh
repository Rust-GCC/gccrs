#!/usr/bin/env bash

set -e

ROOT_DIR=$(git rev-parse --show-toplevel 2>/dev/null || true)
if [ -z "$ROOT_DIR" ]; then
    echo "Warning: You must be inside a git repository to run this command." >&2
    exit 1
fi

if git diff --cached --quiet; then
    echo "Warning: No staged changes found. Run 'git add' first." >&2
    exit 1
fi

TMP_MSG_FILE=$(mktemp -t gccrs-changelog-XXXXXX.txt)
trap 'rm -f "$TMP_MSG_FILE"' EXIT

if ! gccrs-mklog > "$TMP_MSG_FILE"; then
    echo "Warning: Failed to generate ChangeLog template." >&2
    exit 1
fi

if [ ! -s "$TMP_MSG_FILE" ]; then
    echo "Warning: ChangeLog template is empty. Aborting commit." >&2
    exit 1
fi

git commit -t "$TMP_MSG_FILE" "$@"
