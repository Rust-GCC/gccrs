trait A<T>: std::ops::Add<Self> + Sized {}
trait B<T>: A<T> {}
trait C<T>: A<dyn B<T, Output=usize>> {}
// { dg-error ".E0038." "" { target *-*-* } .-1 }

fn main() {}

