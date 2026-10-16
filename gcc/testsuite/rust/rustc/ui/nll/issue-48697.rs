// Regression test for #48697

fn foo(x: &i32) -> &i32 {
    let z = 4;
    let f = &|y| y;
    let k = f(&z);
    f(x) // { dg-error ".E0515." "" { target *-*-* } }
}

fn main() {}

