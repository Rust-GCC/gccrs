// Test that we can't pass other types for !

#![feature(never_type)]

fn foo(x: !) -> ! {
    x
}

fn main() {
    foo("wow"); // { dg-error ".E0308." "" { target *-*-* } }
}

