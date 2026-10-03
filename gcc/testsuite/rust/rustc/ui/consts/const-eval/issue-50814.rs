// build-fail

trait Unsigned {
    const MAX: u8;
}

struct U8(u8);
impl Unsigned for U8 {
    const MAX: u8 = 0xff;
}

struct Sum<A,B>(A,B);

impl<A: Unsigned, B: Unsigned> Unsigned for Sum<A,B> {
    const MAX: u8 = A::MAX + B::MAX;
// { dg-error "" "" { target *-*-* } .-1 }
}

fn foo<T>(_: T) -> &'static u8 {
    &Sum::<U8,U8>::MAX
// { dg-error ".E0080." "" { target *-*-* } .-1 }
}

fn main() {
    foo(0);
}

