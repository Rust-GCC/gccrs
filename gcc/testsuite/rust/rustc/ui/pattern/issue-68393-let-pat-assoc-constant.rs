pub enum EFoo {
    A,
}

pub trait Foo {
    const X: EFoo;
}

struct Abc;

impl Foo for Abc {
    const X: EFoo = EFoo::A;
}

struct Def;
impl Foo for Def {
    const X: EFoo = EFoo::A;
}

pub fn test<A: Foo, B: Foo>(arg: EFoo, A::X: EFoo) {
// { dg-error ".E0158." "" { target *-*-* } .-1 }
    let A::X = arg;
// { dg-error ".E0158." "" { target *-*-* } .-1 }
}

fn main() {}

