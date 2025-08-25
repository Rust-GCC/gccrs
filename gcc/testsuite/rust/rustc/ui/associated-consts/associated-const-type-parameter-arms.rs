pub enum EFoo { A, B, C, D }

pub trait Foo {
    const X: EFoo;
}

struct Abc;

impl Foo for Abc {
    const X: EFoo = EFoo::B;
}

struct Def;
impl Foo for Def {
    const X: EFoo = EFoo::D;
}

pub fn test<A: Foo, B: Foo>(arg: EFoo) {
    match arg {
        A::X => println!("A::X"),
// { dg-error ".E0158." "" { target *-*-* } .-1 }
        B::X => println!("B::X"),
// { dg-error ".E0158." "" { target *-*-* } .-1 }
        _ => (),
    }
}

fn main() {
}

