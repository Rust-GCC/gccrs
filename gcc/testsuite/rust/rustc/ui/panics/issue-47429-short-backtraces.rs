// Regression test for #47429: short backtraces were not terminating correctly

// compile-flags: -O
// run-fail
// check-run-results
// exec-env:RUST_BACKTRACE=1

// ignore-msvc see #62897 and `backtrace-debuginfo.rs` test
// ignore-android FIXME #17520
// ignore-cloudabi spawning processes is not supported
// ignore-openbsd no support for libbacktrace without filename
// ignore-wasm no panic or subprocess support
// ignore-emscripten no panic or subprocess support
// ignore-sgx no subprocess support

fn main() {
    panic!()
}

