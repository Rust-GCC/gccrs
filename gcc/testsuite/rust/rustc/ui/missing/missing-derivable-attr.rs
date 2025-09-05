trait MyEq {
    fn eq(&self, other: &Self) -> bool;
}

struct A {
    x: isize
}

impl MyEq for isize {
    fn eq(&self, other: &isize) -> bool { *self == *other }
}

impl MyEq for A {}  // { dg-error ".E0046." "" { target *-*-* } }

fn main() {
}

