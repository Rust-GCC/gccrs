#!/usr/bin/env sh

set -ex

: "${TARGET?The TARGET environment variable must be set.}"

# Tests are all super fast anyway, and they fault often enough on travis that
# having only one thread increases debuggability to be worth it.
#export RUST_BACKTRACE=full
#export RUST_TEST_NOCAPTURE=1
#export RUST_TEST_THREADS=1

RUSTFLAGS="$RUSTFLAGS -D warnings "

case ${TARGET} in
    # On 32-bit use a static relocation model which avoids some extra
    # instructions when dealing with static data, notably allowing some
    # instruction assertion checks to pass below the 20 instruction limit. If
    # this is the default, dynamic, then too many instructions are generated
    # when we assert the instruction for a function and it causes tests to fail.
    #
    # It's not clear why `-Z plt=yes` is required here. Probably a bug in LLVM.
    # If you can remove it and CI passes, please feel free to do so!
    i686-* | i586-*)
        export RUSTFLAGS="${RUSTFLAGS} -C relocation-model=static -Z plt=yes"
        ;;
    #Unoptimized build uses fast-isel which breaks with msa
    mips-* | mipsel-*)
	export RUSTFLAGS="${RUSTFLAGS} -C llvm-args=-fast-isel=false"
	;;
esac

echo "RUSTFLAGS=${RUSTFLAGS}"
echo "FEATURES=${FEATURES}"
echo "OBJDUMP=${OBJDUMP}"
echo "STDARCH_DISABLE_ASSERT_INSTR=${STDARCH_DISABLE_ASSERT_INSTR}"
echo "STDARCH_TEST_EVERYTHING=${STDARCH_TEST_EVERYTHING}"

cargo_test() {
    cmd="cargo"
    subcmd="test"
    if [ "$NORUN" = "1" ]; then
        export subcmd="build"
    fi
    cmd="$cmd ${subcmd} --target=$TARGET $1"
    cmd="$cmd -- $2"

    # wasm targets can't catch panics so if a test failures make sure the test
    # harness isn't trying to capture output, otherwise we won't get any useful
    # output.
    case ${TARGET} in
        wasm32*)
            cmd="$cmd --nocapture"
            ;;
    esac

    $cmd
}

CORE_ARCH="--manifest-path=crates/core_arch/Cargo.toml"
STD_DETECT="--manifest-path=crates/std_detect/Cargo.toml"
STDARCH_EXAMPLES="--manifest-path=examples/Cargo.toml"
cargo_test "${CORE_ARCH} --release"

if [ "$NOSTD" != "1" ]; then
    cargo_test "${STD_DETECT}"
    cargo_test "${STD_DETECT} --release"

    cargo_test "${STD_DETECT} --no-default-features"
    cargo_test "${STD_DETECT} --no-default-features --features=std_detect_file_io"
    cargo_test "${STD_DETECT} --no-default-features --features=std_detect_dlsym_getauxval"
    cargo_test "${STD_DETECT} --no-default-features --features=std_detect_dlsym_getauxval,std_detect_file_io"

    cargo_test "${STDARCH_EXAMPLES}"
    cargo_test "${STDARCH_EXAMPLES} --release"
fi

# Test targets compiled with extra features.
case ${TARGET} in
    x86*)
        export STDARCH_DISABLE_ASSERT_INSTR=1
        export RUSTFLAGS="${RUSTFLAGS} -C target-feature=+avx"
        cargo_test "--release"
        ;;
    wasm32*)
        prev="$RUSTFLAGS"
        export RUSTFLAGS="${RUSTFLAGS} -C target-feature=+simd128,+unimplemented-simd128"
        cargo_test "--release"
        export RUSTFLAGS="$prev"
        ;;
    # FIXME: don't build anymore
    #mips-*gnu* | mipsel-*gnu*)
    #    export RUSTFLAGS="${RUSTFLAGS} -C target-feature=+msa,+fp64,+mips32r5"
    #    cargo_test "--release"
	  #    ;;
    mips64*)
        export RUSTFLAGS="${RUSTFLAGS} -C target-feature=+msa"
        cargo_test "--release"
	      ;;
    powerpc64*)
        # We don't build the ppc 32-bit targets with these - these targets
        # are mostly unsupported for now.
        OLD_RUSTFLAGS="${RUSTFLAGS}"
        export RUSTFLAGS="${OLD_RUSTFLAGS} -C target-feature=+altivec"
        cargo_test "--release"

        export RUSTFLAGS="${OLD_RUSTFLAGS} -C target-feature=+vsx"
        cargo_test "--release"
        ;;
    *)
        ;;

esac

if [ "$NORUN" != "1" ] && [ "$NOSTD" != 1 ]; then
    # Test examples
    (
        cd examples
        cargo test --target "$TARGET"
        echo test | cargo run --release hex
    )
fi
