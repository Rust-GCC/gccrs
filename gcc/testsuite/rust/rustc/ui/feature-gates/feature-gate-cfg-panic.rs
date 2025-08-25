#[cfg(panic = "unwind")]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn foo() -> bool { true }
#[cfg(not(panic = "unwind"))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn foo() -> bool { false }


fn main() {
    assert!(foo());
}

