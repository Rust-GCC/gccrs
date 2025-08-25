// compile-flags: -Z trace-macros

#![recursion_limit = "4"]

macro_rules! my_faulty_macro {
    () => {
        my_faulty_macro!(bcd); // { dg-error "" "" { target *-*-* } }
    };
}

macro_rules! pat_macro {
    () => {
        pat_macro!(A{a:a, b:0, c:_, ..});
    };
    ($a:pat) => {
        $a // { dg-error "" "" { target *-*-* } }
    };
}

macro_rules! my_recursive_macro {
    () => {
        my_recursive_macro!(); // { dg-error "" "" { target *-*-* } }
    };
}

macro_rules! my_macro {
    () => {};
}

fn main() {
    my_faulty_macro!();
    my_recursive_macro!();
    test!();
    non_exisiting!();
    derive!(Debug);
    let a = pat_macro!();
}

#[my_macro]
fn use_bang_macro_as_attr() {}

#[derive(Debug)] // { dg-error ".E0774." "" { target *-*-* } }
fn use_derive_macro_as_attr() {}

