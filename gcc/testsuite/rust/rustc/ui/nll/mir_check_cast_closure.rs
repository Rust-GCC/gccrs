// compile-flags: -Z borrowck=mir

#![allow(dead_code)]

fn bar<'a, 'b>() -> fn(&'a u32, &'b u32) -> &'a u32 {
    let g: fn(_, _) -> _ = |_x, y| y;
    g
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

