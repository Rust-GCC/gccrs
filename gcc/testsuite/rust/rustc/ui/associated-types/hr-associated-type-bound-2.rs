trait X<'a>
where
    for<'b> <Self as X<'b>>::U: Clone,
{
    type U: ?Sized;
    fn f(&self, x: &Self::U) {
        <Self::U>::clone(x);
    }
}

impl X<'_> for u32
where
    for<'b> <Self as X<'b>>::U: Clone,
{
    type U = str;
}

fn main() {
    1u32.f("abc");
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

