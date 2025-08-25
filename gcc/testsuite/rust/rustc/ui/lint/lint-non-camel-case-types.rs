#![forbid(non_camel_case_types)]
#![allow(dead_code)]

struct ONE_TWO_THREE;
// { dg-error "" "" { target *-*-* } .-1 }

struct foo { // { dg-error "" "" { target *-*-* } }
    bar: isize,
}

enum foo2 { // { dg-error "" "" { target *-*-* } }
    Bar
}

struct foo3 { // { dg-error "" "" { target *-*-* } }
    bar: isize
}

type foo4 = isize; // { dg-error "" "" { target *-*-* } }

enum Foo5 {
    bar // { dg-error "" "" { target *-*-* } }
}

trait foo6 { // { dg-error "" "" { target *-*-* } }
    type foo7; // { dg-error "" "" { target *-*-* } }
    fn dummy(&self) { }
}

fn f<ty>(_: ty) {} // { dg-error "" "" { target *-*-* } }

#[repr(C)]
struct foo7 {
    bar: isize,
}

fn main() { }

