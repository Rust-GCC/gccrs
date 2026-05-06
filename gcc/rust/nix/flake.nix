{
  description = "Reproducible Nix environment for GCC Rust (gccrs) development";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      pkgs32 = pkgs.pkgsi686Linux;

      pythonEnv = pkgs.python3.withPackages (
        ps: with ps; [
          requests
          unidiff
          gitpython
        ]
      );

      # gcc mklog script wrapper
      gccrs-mklog = pkgs.writeShellApplication {
        name = "gccrs-mklog";
        runtimeInputs = [
          pkgs.git
          pythonEnv
        ];
        text = ''
          if [ ! -f "contrib/mklog.py" ]; then
              echo "Warning: 'contrib/mklog.py' not found! Use in 'gccrs' root folder."
              exit 1
          fi

          git diff --cached | python3 contrib/mklog.py -
        '';
      };

      # gcc check gnu commit script wrapper
      gccrs-check-commit = pkgs.writeShellApplication {
        name = "gccrs-check-commit";
        runtimeInputs = [
          pkgs.git
          pythonEnv
        ];
        text = ''
          if [ ! -f "contrib/gcc-changelog/git_check_commit.py" ]; then
              echo "Warning: 'contrib/gcc-changelog/git_check_commit.py' not found. Use in 'gccrs' root folder."
              exit 1
          fi

          TARGET_COMMIT="''${1:-HEAD}"
          python3 contrib/gcc-changelog/git_check_commit.py "$TARGET_COMMIT"        '';
      };

      mkGccrsEnv =
        { is32Bit }:
        let
          targetPkgs = if is32Bit then pkgs32 else pkgs;

          buildDirName = if is32Bit then "build32" else "build";
          installDirName = if is32Bit then "install32" else "install";
          setupName = if is32Bit then "gccrs-setup32" else "gccrs-setup";
          buildName = if is32Bit then "gccrs-build32" else "gccrs-build";
          flakeTarget = if is32Bit then "#gcc32" else "";

          gccrs-setup = pkgs.writeScriptBin setupName ''
            #!/usr/bin/env bash
            set -e

            ROOT_DIR="$PWD"
            SRC_DIR="$ROOT_DIR/gccrs"
            BUILD_DIR="$ROOT_DIR/${buildDirName}"
            INSTALL_DIR="$ROOT_DIR/${installDirName}"

            SKIP_CLONE=false
            SETUP_DIRENV=false

            for arg in "$@"; do
              case $arg in
                --skip-clone) SKIP_CLONE=true ;;
                --use-direnv) SETUP_DIRENV=true ;;
              esac
            done

            if [ -d "$SRC_DIR" ]; then
                if [ "$SKIP_CLONE" = true ]; then
                    echo "Existing 'gccrs' directory found. Skipping clone as requested."
                else
                    echo "Warning: '$SRC_DIR' already exists. Rename or remove it and rerun '${setupName}'!"
                    echo "If it's ready for build, run: 'gccrs-setup --skip-clone'"
                    exit 1
                fi
            else
                if [ "$SKIP_CLONE" = true ]; then
                    echo "Warning: --skip-clone flag used but '$SRC_DIR' not found!"
                    exit 1
                fi
                git clone https://github.com/Rust-GCC/gccrs.git
            fi

            export CC="ccache gcc"
            export CXX="ccache g++"
            export CFLAGS="-g -O0"
            export CXXFLAGS="-g -O0"

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

            make -j "$(nproc)" \
                 WERROR="" \
                 STRICT_FLAGS="" \
                 CFLAGS="-Wno-error -Wno-format-security" \
                 CXXFLAGS="-Wno-error -Wno-format-security"

            make install

            if [ "$SETUP_DIRENV" = true ]; then
                if [ -f "$ROOT_DIR/flake.nix" ]; then
                    echo "use flake \"../${flakeTarget}\"" > .envrc
                else
                    ##############################################################################
                    ## change this with Rust-GCC
                    ##############################################################################
                    #VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
                    echo "use flake \"github:nsvke/gccrs.nix${flakeTarget}\"" > .envrc
                fi
                echo "Run 'direnv allow' inside '${buildDirName}' to activate direnv."
            fi

            echo "Your gccrs build is ready!"
            echo "Use '${buildName}' or 'make' commands to compile it again."
          '';

          gccrs-build = pkgs.writeScriptBin buildName ''
            #!/usr/bin/env bash
            set -e

            if [[ "$(basename "$PWD")" == "${buildDirName}" ]]; then
                BUILD_DIR="$PWD"
            else
                BUILD_DIR="$PWD/${buildDirName}"
                if [ ! -d "$BUILD_DIR" ]; then
                    echo "Warning: '${buildDirName}' folder does not exist. Run '${setupName}' first."
                    exit 1
                fi
                cd "$BUILD_DIR"
            fi

            make -j "$(nproc)" \
                 WERROR="" \
                 STRICT_FLAGS="" \
                 CFLAGS="-Wno-error -Wno-format-security" \
                 CXXFLAGS="-Wno-error -Wno-format-security" "$@"
          '';
        in
        targetPkgs.mkShell {
          name = "gccrs-dev${if is32Bit then "-32" else "-64"}";

          packages = [
            gccrs-setup
            gccrs-build
            gccrs-mklog
            gccrs-check-commit
          ];

          nativeBuildInputs = with targetPkgs; [
            # compile dependices
            gnumake
            pkg-config
            autoconf
            automake
            m4
            flex
            bison
            texinfo
            ccache

            # test dependencies
            dejagnu
            expect
            tcl
            autogen
            check

            # debug dependencies
            gdb
            valgrind
            strace

            # rust dependencies
            cargo
            rustc
          ];

          buildInputs = with targetPkgs; [
            gmp
            mpfr
            libmpc
            isl
            zlib
            zstd
            gettext
          ];

          shellHook = ''
            export GCCRS_INCOMPLETE_AND_EXPERIMENTAL_COMPILER_DO_NOT_USE="1"
            export NIX_GLIBC_INCLUDE="${targetPkgs.glibc.dev}/include"
            export LIBRARY_PATH="${targetPkgs.glibc}/lib"

            echo "🦀 GCC Rust (gccrs) ${if is32Bit then "32-bit" else "64-bit"} Development Environment"
            echo "Run '${setupName}' to start from scratch."
            echo "Run '${setupName} --skip-clone' if you already have the 'gccrs' folder."
            echo "Run '${setupName} --use-direnv' to automatically setup direnv your '${buildDirName}' folder."
            echo "Run '${buildName}' for incremental builds."
            echo "Run 'gccrs-mklog' to generate a changelog for your changes."
            echo "Run 'gccrs-check-commit' to verify your commits against GNU standards."
          '';

        };

    in
    {
      devShells.${system} = {

        default = mkGccrsEnv { is32Bit = false; };

        gcc32 = mkGccrsEnv { is32Bit = true; };

      };
    };
}
