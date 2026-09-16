trait X<'a>
where
    for<'b> <Self as X<'b>>::U: Clone,
{
    type U: ?Sized;
}
fn f<'a, T: X<'a> + ?Sized>(x: &<T as X<'a>>::U) {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    <<T as X<'_>>::U>::clone(x);
}

pub fn main() {
    f::<dyn X<'_, U = str>>("abc");
}

