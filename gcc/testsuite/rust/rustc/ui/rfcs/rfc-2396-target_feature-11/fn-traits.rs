// only-x86_64

#![feature(target_feature_11)]

#[target_feature(enable = "avx")]
fn foo() {}

#[target_feature(enable = "avx")]
unsafe fn foo_unsafe() {}

fn call(f: impl Fn()) {
    f()
}

fn call_mut(f: impl FnMut()) {
    f()
}

fn call_once(f: impl FnOnce()) {
    f()
}

fn main() {
    call(foo); // { dg-error ".E0277." "" { target *-*-* } }
    call_mut(foo); // { dg-error ".E0277." "" { target *-*-* } }
    call_once(foo); // { dg-error ".E0277." "" { target *-*-* } }

    call(foo_unsafe);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    call_mut(foo_unsafe);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    call_once(foo_unsafe);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

