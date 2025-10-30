use std::ops::Add;

pub trait Encoder {
    type Size: Add<Output = Self::Size>;

    fn foo(&self) -> Self::Size;
}

pub trait SubEncoder: Encoder {
    type ActualSize;

    fn bar(&self) -> Self::Size;
}

impl<T> Encoder for T
where
    T: SubEncoder,
{
    type Size = <Self as SubEncoder>::ActualSize;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    fn foo(&self) -> Self::Size {
        self.bar() + self.bar()
    }
}

pub struct UnitEncoder;

impl SubEncoder for UnitEncoder {
    type ActualSize = ();

    fn bar(&self) {}
}

pub fn fun<R: Encoder>(encoder: &R) {
    encoder.foo();
}

fn main() {
    fun(&UnitEncoder {});
}

