// Test equality constraints on associated types. Check we get type errors
// where we should.

pub trait Foo {
    type A;
    fn boo(&self) -> <Self as Foo>::A;
}

struct Bar;

impl Foo for isize {
    type A = usize;
    fn boo(&self) -> usize {
        42
    }
}

fn foo1<I: Foo<A=Bar>>(x: I) {
    let _: Bar = x.boo();
}

fn foo2<I: Foo>(x: I) {
    let _: Bar = x.boo();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}


pub fn baz(x: &dyn Foo<A=Bar>) {
    let _: Bar = x.boo();
}


pub fn main() {
    let a = 42;
    foo1(a);
// { dg-error ".E0271." "" { target *-*-* } .-1 }
// { dg-error ".E0271." "" { target *-*-* } .-2 }
    baz(&a);
// { dg-error ".E0271." "" { target *-*-* } .-1 }
// { dg-error ".E0271." "" { target *-*-* } .-2 }
}

