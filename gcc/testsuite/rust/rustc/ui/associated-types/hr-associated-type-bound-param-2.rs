// ignore-compare-mode-chalk
trait Z<'a, T: ?Sized>
where
    T: Z<'a, u16>,
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
    for<'b> <T as Z<'b, u16>>::W: Clone,
{
    type W: ?Sized;
    fn h(&self, x: &T::W) {
        <T::W>::clone(x);
    }
}

impl<'a> Z<'a, u16> for u16 {
    type W = str;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    1u16.h("abc");
}

