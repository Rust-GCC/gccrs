#[cfg(version("1.44"))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn foo() -> bool { true }
#[cfg(not(version("1.44")))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn foo() -> bool { false }

#[cfg(version("1.43", "1.44", "1.45"))] // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool  { false }
#[cfg(version(false))] // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool  { false }
#[cfg(version("foo"))] // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool  { false }
#[cfg(version("999"))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool  { false }
#[cfg(version("-1"))] // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool  { false }
#[cfg(version("65536"))] // { dg-error ".E0658." "" { target *-*-* } }
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool  { false }
#[cfg(version("0"))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn bar() -> bool { true }

#[cfg(version("1.65536.2"))]
// { dg-error ".E0658." "" { target *-*-* } .-1 }
fn version_check_bug() {}

fn main() {
    // This should fail but due to a bug in version_check `1.65536.2` is interpreted as `1.2`.
    // See https://github.com/SergioBenitez/version_check/issues/11
    version_check_bug();
    assert!(foo());
    assert!(bar());
    assert!(cfg!(version("1.42"))); // { dg-error ".E0658." "" { target *-*-* } }
}

