{
  writeScriptBin,
  symlinkJoin,
  is32Bit,
}:

let
  buildDirName = if is32Bit then "build32" else "build";
  installDirName = if is32Bit then "install32" else "install";
  setupName = if is32Bit then "gccrs-setup32" else "gccrs-setup";
  buildName = if is32Bit then "gccrs-build32" else "gccrs-build";

  gccrs-setup = writeScriptBin setupName ''
    #!/usr/bin/env bash
    set -e

    if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
      SRC_DIR=$(git rev-parse --show-toplevel)
      ROOT_DIR=$(dirname "$SRC_DIR")
    elif [ -d "$PWD/gccrs" ]; then
      ROOT_DIR="$PWD"
      SRC_DIR="$ROOT_DIR/gccrs"
    else
      echo "Warning: Cannot find 'gccrs' directory." >&2
      echo "Run this command from the workspace root or inside the gccrs repository." >&2
      exit 1
    fi


    BUILD_DIR="$ROOT_DIR/${buildDirName}"
    INSTALL_DIR="$ROOT_DIR/${installDirName}"

    SETUP_DIRENV=false

    for arg in "$@"; do
      case $arg in
        --use-direnv) SETUP_DIRENV=true ;;
      esac
    done

    if [ -f "$BUILD_DIR/Makefile" ]; then
        echo "Warning: '$BUILD_DIR' is already configured!" >&2
        echo "If you want to reconfigure, use 'rm -rf $BUILD_DIR/*'" >&2
        exit 1
    fi

    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"

    "$SRC_DIR/configure" \
        --prefix="$INSTALL_DIR" \
        --disable-bootstrap \
        --enable-valgrind-annotations \
        --enable-languages=rust \
        --disable-multilib \
        --with-native-system-header-dir="$NIX_GLIBC_INCLUDE" \
        --disable-libatomic \
        --disable-libgomp \
        --disable-libquadmath \
        --disable-libssp \
        --disable-libvtv \
        --disable-libitm \
        --disable-libsanitizer \
        ${if is32Bit then "--host=i686-pc-linux-gnu --build=i686-pc-linux-gnu" else ""}

    bear -- make -j "$(nproc)" \
         WERROR="" \
         STRICT_FLAGS="" \
         CFLAGS="-Wno-error -Wno-format-security" \
         CXXFLAGS="-Wno-error -Wno-format-security"

    if [ ! -e "$SRC_DIR/compile_commands.json" ] && [ -f "$BUILD_DIR/compile_commands.json" ]; then
      ln -sf "../${buildDirName}/compile_commands.json" "$SRC_DIR/compile_commands.json"
    fi

    if [ ! -f "$SRC_DIR/.clang-format" ] && [ -f "$SRC_DIR/contrib/clang-format" ]; then
      ln -sf "contrib/clang-format" "$SRC_DIR/.clang-format"
    fi

    if [ "$SETUP_DIRENV" = true ]; then
      ${
        if is32Bit then
          ''
            echo "use flake \"../$(basename "$SRC_DIR")/contrib/nix#gcc32\"" > "$BUILD_DIR/.envrc"
            echo "Run 'direnv allow' inside '${buildDirName}' to activate 32-bit direnv."
          ''
        else
          ''
            echo "use flake \"./$(basename "$SRC_DIR")/contrib/nix\"" > "$ROOT_DIR/.envrc"
            echo "Run 'direnv allow' inside your project root to activate 64-bit direnv."
          ''
      }
    fi

    echo "Your gccrs build is ready!"
    echo "Use '${buildName}' or 'make' commands to compile it again."
  '';

  getBuildDirFunc = ''
    get_build_dir() {
      local curr="$PWD"

      while [[ "$curr" != "/" ]]; do
          if [[ "$(basename "$curr")" == "${buildDirName}" ]]; then
              echo "$curr"
              return 0
          fi
          curr="$(dirname "$curr")"
      done

      if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
          local git_root
          git_root=$(git rev-parse --show-toplevel)

          if [ -d "$(dirname "$git_root")/${buildDirName}" ]; then
            echo "$(dirname "$git_root")/${buildDirName}"
            return 0
          fi
      fi

      if [ -d "$PWD/${buildDirName}" ]; then
          echo "$PWD/${buildDirName}"
          return 0
      fi

      return 1
    }
  '';

  gccrs-build = writeScriptBin buildName ''
    #!/usr/bin/env bash
    set -e

    ${getBuildDirFunc}

    BUILD_DIR=$(get_build_dir)

    if [ -z "$BUILD_DIR" ] || [ ! -d "$BUILD_DIR" ]; then
      echo "Warning: '${buildDirName}' not found!" >&2
      exit 1
    fi

    cd "$BUILD_DIR" || exit 1

    USE_BEAR=false
    USE_INSTRUMENT=false
    while [[ $# -gt 0 ]]; do
      case "$1" in
        --bear)
        USE_BEAR=true
        shift
        ;;
        --instrument)
        USE_INSTRUMENT=true
        shift
        ;;
      *)
        break
        ;;
      esac
    done

    CXXFLAGS="-Wno-error -Wno-format-security"

    if [ "$USE_INSTRUMENT" = true ]; then
      CXXFLAGS="$CXXFLAGS -finstrument-functions -fno-inline"
    fi

    if [ "$USE_BEAR" = true ]; then
      bear --append -- make -j "$(nproc)" \
           WERROR="" \
           STRICT_FLAGS="" \
           CFLAGS="-Wno-error -Wno-format-security" \
           CXXFLAGS="$CXXFLAGS" "$@"
    else
      make -j "$(nproc)" \
           WERROR="" \
           STRICT_FLAGS="" \
           CFLAGS="-Wno-error -Wno-format-security" \
           CXXFLAGS="$CXXFLAGS" "$@"
    fi
  '';

  gx = writeScriptBin "gx" ''
    #!/usr/bin/env bash
    set -e

    ${getBuildDirFunc}

    BUILD_DIR=$(get_build_dir)

    if [ -z "$BUILD_DIR" ]; then
        echo "Warning: Build directory '${buildDirName}' not found!" >&2
        exit 1
    fi

    if [ -z "$1" ]; then
        echo "Usage: gx <binary> [args...]" >&2
        exit 1
    fi

    BIN_NAME="$1"
    shift

    TARGET_BIN="$BUILD_DIR/gcc/$BIN_NAME"

    if [ ! -x "$TARGET_BIN" ]; then
        echo "Warning: Executable '$TARGET_BIN' not found." >&2
        exit 1
    fi

    exec "$TARGET_BIN" "$@"
  '';

  gccrs-test = writeScriptBin "gccrs-test" ''
    #!/usr/bin/env bash
    set -e

    ${getBuildDirFunc}

    BUILD_DIR=$(get_build_dir)

    if [ -z "$BUILD_DIR" ]; then
        echo "Warning: Build directory '${buildDirName}' not found!" >&2
        exit 1
    fi

    BUILD_FIRST=false
    RUNTESTFLAGS=""

    while [[ $# -gt 0 ]]; do
      case $1 in
        --build)
          BUILD_FIRST=true
          shift
          ;;
        *)
          if [ -z "$RUNTESTFLAGS" ]; then
            RUNTESTFLAGS="$1"
          else
            RUNTESTFLAGS="$RUNTESTFLAGS $1"
          fi
          shift
          ;;
      esac
    done

    if [ "$BUILD_FIRST" = true ]; then
        gccrs-build
    fi

    cd "$BUILD_DIR" || exit 1

    make -k -j "$(nproc)" check-rust RUNTESTFLAGS="$RUNTESTFLAGS"
  '';

  gccrs-help = writeScriptBin "gccrs-help" ''
    #!/usr/bin/env bash

    echo "  ${setupName}                  : Start from scratch (configures, builds)."
    echo "  ${setupName} --use-direnv     : Automatically setup direnv for your build folder."
    echo "  ${buildName}                  : Run incremental build (compiles only changes)."
    echo "  ${buildName} --bear           : Incremental build and update compile_commands.json."
    echo "  gccrs-test [args...]          : Run gccrs testsuite."
    echo "  gccrs-test --build [args...]  : Build and run gccrs testsuite."
    echo "  gx <binary> [args...]         : Use built gccrs binaries."
    echo ""
    echo "  gccrs-mklog                  : Generate ChangeLog template for STAGED files."
    echo "  gccrs-commit-mklog           : Generate ChangeLog and open Git commit editor."
    echo "  gccrs-verify                 : Verify your latest commit (HEAD) against GNU std."
    echo "  gccrs-verify <hash>          : Verify a specific commit in your history."
    echo "  gccrs-fix-changelog          : Attempt to fix minor format typos in HEAD."
    echo ""
    echo "  gccrs-style                  : Check GNU C++ style for modifications in HEAD."
    echo "  gccrs-style <hash>           : Check style for modifications in a specific commit."
  '';
in
symlinkJoin {
  name = "gccrs-env-tools-${if is32Bit then "32" else "64"}";
  paths = [
    gccrs-setup
    gccrs-build
    gccrs-test
    gx
    gccrs-help
  ];
}
