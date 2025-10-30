#![feature(const_in_array_repeat_expressions)]

// Some type that is not copyable.
struct Bar;

const fn no_copy() -> Option<Bar> {
    None
}

const fn copy() -> u32 {
    3
}

fn main() {
    let _: [u32; 2] = [copy(); 2];
    let _: [Option<Bar>; 2] = [no_copy(); 2];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

