struct S;

impl S {
    fn f(self: _) {} // { dg-error ".E0121." "" { target *-*-* } }
    fn g(self: &_) {} // { dg-error ".E0121." "" { target *-*-* } }
}

fn main() {}

