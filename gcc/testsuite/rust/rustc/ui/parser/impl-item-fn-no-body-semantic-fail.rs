fn main() {}

struct X;

impl X {
    fn f(); // { dg-error "" "" { target *-*-* } }
}

