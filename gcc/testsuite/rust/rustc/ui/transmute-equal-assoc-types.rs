trait Foo {
    type Bar;
}

unsafe fn noop<F: Foo>(foo: F::Bar) -> F::Bar {
    ::std::mem::transmute(foo) // { dg-error ".E0512." "" { target *-*-* } }
}

fn main() {}

