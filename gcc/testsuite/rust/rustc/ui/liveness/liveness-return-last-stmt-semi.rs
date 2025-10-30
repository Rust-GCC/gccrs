//
// regression test for #8005

macro_rules! test { () => { fn foo() -> i32 { 1; } } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }

fn no_return() -> i32 {} // { dg-error ".E0308." "" { target *-*-* } }

fn bar(x: u32) -> u32 { // { dg-error ".E0308." "" { target *-*-* } }
    x * 2;
}

fn baz(x: u64) -> u32 { // { dg-error ".E0308." "" { target *-*-* } }
    x * 2;
}

fn main() {
    test!();
}

