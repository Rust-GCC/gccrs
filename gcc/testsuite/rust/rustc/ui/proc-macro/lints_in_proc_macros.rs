// aux-build:bang_proc_macro2.rs

extern crate bang_proc_macro2;

use bang_proc_macro2::bang_proc_macro2;

fn main() {
    let foobar = 42;
    bang_proc_macro2!();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { help ".E0425." "" { target *-*-* } .-2 }
// { suggestion ".E0425." "" { target *-*-* } .-3 }
    println!("{}", x);
}

