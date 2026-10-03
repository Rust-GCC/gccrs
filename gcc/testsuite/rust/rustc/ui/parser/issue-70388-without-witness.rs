// run-rustfix
// This is for checking if we can apply suggestions as-is.

pub struct Foo(i32);

fn main() {
    let Foo(...) = Foo(0); // { dg-error "" "" { target *-*-* } }
    let [_, ..., _] = [0, 1]; // { dg-error "" "" { target *-*-* } }
}

