#![feature(box_patterns)]
#![feature(unsized_fn_params)]

#[allow(dead_code)]
fn f1(box box _b: Box<Box<[u8]>>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn f2((_x, _y): (i32, [i32])) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
    let foo: Box<[u8]> = Box::new(*b"foo");
    let _foo: [u8] = *foo;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

