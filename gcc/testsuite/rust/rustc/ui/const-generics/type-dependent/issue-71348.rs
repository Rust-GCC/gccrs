// [full] run-pass
// revisions: full min
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Foo {
    i: i32,
}

trait Get<'a, const N: &'static str> {
// { dg-error "" "" { target *-*-* } .-1 }
    type Target: 'a;

    fn get(&'a self) -> &'a Self::Target;
}

impl Foo {
    fn ask<'a, const N: &'static str>(&'a self) -> &'a <Self as Get<N>>::Target
// { dg-error "" "" { target *-*-* } .-1 }
    where
        Self: Get<'a, N>,
    {
        self.get()
    }
}

impl<'a> Get<'a, "int"> for Foo {
    type Target = i32;

    fn get(&'a self) -> &'a Self::Target {
        &self.i
    }
}

fn main() {
    let foo = Foo { i: 123 };
    assert_eq!(foo.ask::<"int">(), &123);
}

