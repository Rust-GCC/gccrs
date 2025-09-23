// run-rustfix
#![allow(unused_variables)]
#![allow(dead_code)]
#![allow(unused_must_use)]

fn foo() -> i32 {
    {2} + {2} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn bar() -> i32 {
    {2} + 2 // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn zul() -> u32 {
    let foo = 3;
    { 42 } + foo; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    32
}

fn baz() -> i32 {
    { 3 } * 3 // { dg-error ".E0614." "" { target *-*-* } }
// { dg-error ".E0614." "" { target *-*-* } .-1 }
}

fn moo(x: u32) -> bool {
    match x {
        _ => 1,
    } > 0 // { dg-error "" "" { target *-*-* } }
}

fn main() {}

