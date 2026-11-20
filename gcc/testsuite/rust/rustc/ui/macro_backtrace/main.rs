// Test that the macro backtrace facility works
// aux-build:ping.rs
// revisions: default -Zmacro-backtrace
//[-Zmacro-backtrace] compile-flags: -Z macro-backtrace

#[macro_use] extern crate ping;

// a local macro
macro_rules! pong {
    () => { syntax error };
}
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

#[allow(non_camel_case_types)]
struct syntax;

fn main() {
    pong!();
    ping!();
    deep!();
}

