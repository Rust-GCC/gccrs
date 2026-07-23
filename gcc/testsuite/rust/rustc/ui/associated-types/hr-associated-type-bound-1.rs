trait X<'a>
where
    for<'b> <Self as X<'b>>::U: Clone,
{
    type U: ?Sized;
    fn f(&self, x: &Self::U) {
        <Self::U>::clone(x);
    }
}

impl X<'_> for i32 {
    type U = str;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    1i32.f("abc");
}

