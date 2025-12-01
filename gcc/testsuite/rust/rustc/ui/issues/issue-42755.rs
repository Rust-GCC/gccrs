macro_rules! foo {
    ($($p:vis)*) => {} // { dg-error "" "" { target *-*-* } }
}

foo!(a);

fn main() {}

