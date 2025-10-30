// Check that we validate associated type bounds on super traits for trait
// objects

trait Super {
    type Y: Clone;
}

trait X: Super {}

fn f<T: X + ?Sized>() {
    None::<T::Y>.clone();
}

fn main() {
    f::<dyn X<Y = str>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

