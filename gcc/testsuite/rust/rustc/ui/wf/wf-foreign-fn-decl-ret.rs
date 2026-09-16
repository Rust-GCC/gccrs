pub trait Unsatisfied {}

#[repr(transparent)]
pub struct Bar<T: Unsatisfied>(T);

pub trait Foo {
    type Assoc;
}

extern "C" {
    pub fn lint_me() -> <() as Foo>::Assoc;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    pub fn lint_me_aswell() -> Bar<u32>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

