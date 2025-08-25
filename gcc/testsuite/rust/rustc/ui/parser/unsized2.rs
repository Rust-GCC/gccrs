// Test syntax checks for `type` keyword.

fn f<X>() {}

pub fn main() {
    f<type>(); // { dg-error "" "" { target *-*-* } }
}

