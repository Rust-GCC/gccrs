// run-rustfix

pub struct Struct {
    pub a: usize,
};
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

