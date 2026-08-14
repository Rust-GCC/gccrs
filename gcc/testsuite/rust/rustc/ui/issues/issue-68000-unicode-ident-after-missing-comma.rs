pub struct Foo {
    pub bar: Vec<i32>ö
// { dg-error "" "" { target *-*-* } .-1 }
} // { dg-error "" "" { target *-*-* } }

fn main() {}

