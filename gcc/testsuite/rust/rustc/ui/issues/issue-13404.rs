use a::f;
use b::f; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

mod a { pub fn f() {} }
mod b { }

fn main() {
    f();
}

