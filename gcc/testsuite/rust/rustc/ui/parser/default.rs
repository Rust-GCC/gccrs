// Test successful and unsuccessful parsing of the `default` contextual keyword

#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait Foo {
    fn foo<T: Default>() -> T;
}

impl Foo for u8 {
    default fn foo<T: Default>() -> T {
        T::default()
    }
}

impl Foo for u16 {
    pub default fn foo<T: Default>() -> T { // { dg-error ".E0449." "" { target *-*-* } }
        T::default()
    }
}

impl Foo for u32 { // { dg-error ".E0046." "" { target *-*-* } }
    default pub fn foo<T: Default>() -> T { T::default() }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

