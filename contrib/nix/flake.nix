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


      mkGccrsEnv =
        { is32Bit ? false, useShortPrefix ? false }:
        let
          targetPkgs = if is32Bit then pkgs32 else pkgs;

          gccrs-env-tools = targetPkgs.callPackage ./packages/gccrs-env-tools.nix {
            inherit is32Bit useShortPrefix;
          };

          gccrs-contrib-tools = pkgs.callPackage ./packages/gccrs-contrib-tools.nix {
            inherit useShortPrefix;
          };
        in
        targetPkgs.mkShell.override { stdenv = targetPkgs.ccacheStdenv; } {
          name = "gccrs-dev${if is32Bit then "-32" else "-64"}";

          packages = [
            gccrs-env-tools
            gccrs-contrib-tools
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
            mold

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
            uftrace

            # rust dependencies
            cargo
            rustc

            # format dependencies
            pkgs.bear
            pkgs.clang-tools
            pkgs.rustfmt
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
            export NIX_CFLAGS_LINK="-fuse-ld=mold"
            export GCCRS_INCOMPLETE_AND_EXPERIMENTAL_COMPILER_DO_NOT_USE="1"
            export NIX_GLIBC_INCLUDE="${targetPkgs.glibc.dev}/include"
            export LIBRARY_PATH="${targetPkgs.glibc}/lib"

            echo "🦀 GCC Rust (gccrs) ${if is32Bit then "32-bit" else "64-bit"} Development Environment"
            echo "Run '${if useShortPrefix then "g" else "gccrs-"}help' to see all available commands"
          '';

        };

    in
    {
      devShells.${system} = {

        default = mkGccrsEnv {};

        gcc32 = mkGccrsEnv { is32Bit = true; };

        gprefix = mkGccrsEnv { useShortPrefix = true; };

        gprefix32 = mkGccrsEnv { is32Bit = true; useShortPrefix = true; };

      };
    };
}
