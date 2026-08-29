pub trait Resources<'a> {}

pub trait Buffer<'a, R: Resources<'a>> {

    fn select(&self) -> BufferViewHandle<R>;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

pub struct BufferViewHandle<'a, R: 'a+Resources<'a>>(&'a R);

fn main() {}

