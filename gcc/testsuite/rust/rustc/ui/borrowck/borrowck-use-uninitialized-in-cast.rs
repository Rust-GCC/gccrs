// Check that we detect unused values that are cast to other things.
// The problem was specified to casting to `*`, as creating unsafe
// pointers was not being fully checked. Issue #20791.

fn main() {
    let x: &i32;
    let y = x as *const i32; // { dg-error ".E0381." "" { target *-*-* } }
}

