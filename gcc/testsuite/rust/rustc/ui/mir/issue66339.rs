// compile-flags: -Z mir-opt-level=2
// build-pass

// This used to ICE in const-prop

fn foo() {
    let bar = |_| { };
    let _ = bar("a");
}

fn main() {
    foo();
}

