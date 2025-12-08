// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

trait Baz {
    type Quaks;
}
impl Baz for u8 {
    type Quaks = [u16; 3];
}

trait Bar {}
impl Bar for [u16; 4] {}
impl Bar for [[u16; 3]; 3] {}

trait Foo  // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    where
        [<u8 as Baz>::Quaks; 2]: Bar,
        <u8 as Baz>::Quaks: Bar,
{
}

struct FooImpl;

impl Foo for FooImpl {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn f(_: impl Foo) {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

fn main() {
    f(FooImpl)
}

