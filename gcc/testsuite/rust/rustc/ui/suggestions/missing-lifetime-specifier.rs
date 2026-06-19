#![allow(bare_trait_objects)]
use std::collections::HashMap;
use std::cell::RefCell;

pub union Foo<'t, 'k> {
    i: &'t i64,
    f: &'k f64,
}
trait Bar<'t, 'k> {}

pub union Qux<'t, 'k, I> {
    i: &'t I,
    f: &'k I,
}
trait Tar<'t, 'k, I> {}

thread_local! {
    static a: RefCell<HashMap<i32, Vec<Vec<Foo>>>> = RefCell::new(HashMap::new());
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
}
thread_local! {
    static b: RefCell<HashMap<i32, Vec<Vec<&Bar>>>> = RefCell::new(HashMap::new());
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
// { dg-error ".E0106." "" { target *-*-* } .-3 }
// { dg-error ".E0106." "" { target *-*-* } .-4 }
}
thread_local! {
    static c: RefCell<HashMap<i32, Vec<Vec<Qux<i32>>>>> = RefCell::new(HashMap::new());
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
}
thread_local! {
    static d: RefCell<HashMap<i32, Vec<Vec<&Tar<i32>>>>> = RefCell::new(HashMap::new());
// { dg-error ".E0106." "" { target *-*-* } .-1 }
// { dg-error ".E0106." "" { target *-*-* } .-2 }
// { dg-error ".E0106." "" { target *-*-* } .-3 }
// { dg-error ".E0106." "" { target *-*-* } .-4 }
}

thread_local! {
    static e: RefCell<HashMap<i32, Vec<Vec<Qux<'static, i32>>>>> = RefCell::new(HashMap::new());
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }
// { dg-error ".E0107." "" { target *-*-* } .-4 }
}
thread_local! {
    static f: RefCell<HashMap<i32, Vec<Vec<&Tar<'static, i32>>>>> = RefCell::new(HashMap::new());
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }
// { dg-error ".E0107." "" { target *-*-* } .-4 }
// { dg-error ".E0107." "" { target *-*-* } .-5 }
// { dg-error ".E0107." "" { target *-*-* } .-6 }
}

fn main() {}

