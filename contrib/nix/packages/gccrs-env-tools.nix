{
  writeScriptBin,
  symlinkJoin,
  is32Bit,
  useShortPrefix,
}:

let
  buildDirName = if is32Bit then "build32" else "build";
  installDirName = if is32Bit then "install32" else "install";
  prefix = if useShortPrefix then "g" else "gccrs-";
  suffix = if is32Bit then "32" else "";
  setupName = "${prefix}setup${suffix}";
  buildName = "${prefix}build${suffix}";
  execName = if useShortPrefix then "gx${suffix}" else "gccrs-exec${suffix}";
  execHelpSpace = if useShortPrefix then "   " else "";
  testName = "${prefix}test${suffix}";
  helpName = "${prefix}help${suffix}";
  gitName = "${prefix}git";

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
    FLAKE_TARGET="${if useShortPrefix then "#gprefix${if is32Bit then "32" else ""}" else if is32Bit then "#gcc32" else ""}"

    for arg in "$@"; do
      case $arg in
        --use-direnv) SETUP_DIRENV=true ;;
        --use-gprefix) FLAKE_TARGET="#gprefix${if is32Bit then "32" else""}" ;;
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
            echo "use flake \"../$(basename "$SRC_DIR")/contrib/nix$FLAKE_TARGET\"" > "$BUILD_DIR/.envrc"
            echo "Run 'direnv allow' inside '${buildDirName}' to activate 32-bit direnv."
          ''
        else
          ''
            echo "use flake \"./$(basename "$SRC_DIR")/contrib/nix$FLAKE_TARGET\"" > "$ROOT_DIR/.envrc"
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

  gccrs-exec = writeScriptBin execName ''
    #!/usr/bin/env bash
    set -e

    ${getBuildDirFunc}

    BUILD_DIR=$(get_build_dir)

    if [ -z "$BUILD_DIR" ]; then
        echo "Warning: Build directory '${buildDirName}' not found!" >&2
        exit 1
    fi

    if [ -z "$1" ]; then
        echo "Usage: ${execName} <binary> [args...]" >&2
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

  gccrs-test = writeScriptBin testName ''
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
        ${buildName}
    fi

    cd "$BUILD_DIR" || exit 1

    make -k -j "$(nproc)" check-rust RUNTESTFLAGS="$RUNTESTFLAGS"
  '';

  gccrs-git = writeScriptBin gitName ''
    #!/usr/bin/env bash
    set -e

    ${getBuildDirFunc}

    BUILD_DIR=$(get_build_dir)

    if [ -z "$BUILD_DIR" ]; then
        echo "Warning: Build directory '${buildDirName}' not found!" >&2
        exit 1
    fi

    REPO_NAME="''${GCCRS_REPO:-gccrs}"
    GIT_DIR="$BUILD_DIR/../$REPO_NAME"

    if [ ! -d "$GIT_DIR" ]; then
        echo "Warning: Directory '$GIT_DIR' not found." >&2
        echo "Help: If you are using a custom directory name, override it like this:" >&2
        echo "      GCCRS_REPO=\"customdir\" ${gitName} <args...>" >&2
        exit 1
    fi
    
    exec git -C "$GIT_DIR" "$@"
  '';

  gccrs-help = writeScriptBin helpName ''
    #!/usr/bin/env bash

    echo "  ${setupName}                   : Start from scratch (configures, builds)."
    echo "  ${setupName} --use-direnv      : Automatically setup direnv for your build folder."
    echo "  ${setupName} --use-gprefix     : Use prefix as 'g' instead of 'gccrs-'."
    echo "  ${buildName}                   : Run incremental build (compiles only changes)."
    echo "  ${buildName} --bear            : Incremental build and update compile_commands.json."
    echo "  ${testName} <args...>          : Run gccrs testsuite."
    echo "  ${testName} --build <args...>  : Build and run gccrs testsuite."
    echo "  ${execName} <binary> <args...> ${execHelpSpace}: Use built gccrs binaries."
    echo "  ${gitName} <args...>           : Use git within the gccrs source directory."
    echo ""
    echo "  ${prefix}mklog                   : Generate ChangeLog template for STAGED files."
    echo "  ${prefix}commit-mklog            : Generate ChangeLog and open Git commit editor."
    echo "  ${prefix}verify                  : Verify your latest commit (HEAD) against GNU std."
    echo "  ${prefix}verify <hash>           : Verify a specific commit in your history."
    echo "  ${prefix}fix-changelog           : Attempt to fix minor format typos in HEAD."
    echo ""
    echo "  ${prefix}style                   : Check GNU C++ style for modifications in HEAD."
    echo "  ${prefix}style <hash>            : Check style for modifications in a specific commit."
  '';
in
symlinkJoin {
  name = "gccrs-env-tools-${if is32Bit then "32" else "64"}";
  paths = [
    gccrs-setup
    gccrs-build
    gccrs-test
    gccrs-exec
    gccrs-git
    gccrs-help
  ];
}
