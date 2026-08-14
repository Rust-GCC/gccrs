#![allow(dead_code)]
#![deny(unused_results, unused_must_use)]
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

#[must_use]
enum MustUse { Test }

#[must_use = "some message"]
enum MustUseMsg { Test2 }

fn foo<T>() -> T { panic!() }

fn bar() -> isize { return foo::<isize>(); }
fn baz() -> MustUse { return foo::<MustUse>(); }
fn qux() -> MustUseMsg { return foo::<MustUseMsg>(); }

#[allow(unused_results)]
fn test() {
    foo::<isize>();
    foo::<MustUse>(); // { dg-error "" "" { target *-*-* } }
    foo::<MustUseMsg>(); // { dg-error "" "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-1 }
}

#[allow(unused_results, unused_must_use)]
fn test2() {
    foo::<isize>();
    foo::<MustUse>();
    foo::<MustUseMsg>();
}

fn main() {
    foo::<isize>(); // { dg-error "" "" { target *-*-* } }
    foo::<MustUse>(); // { dg-error "" "" { target *-*-* } }
    foo::<MustUseMsg>(); // { dg-error "" "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-1 }

    let _ = foo::<isize>();
    let _ = foo::<MustUse>();
    let _ = foo::<MustUseMsg>();
}

