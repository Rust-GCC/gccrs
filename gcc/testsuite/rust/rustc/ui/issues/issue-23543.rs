pub trait A: Copy {}

struct Foo;

pub trait D {
    fn f<T>(self)
        where T<Bogus = Foo>: A;
// { dg-error ".E0229." "" { target *-*-* } .-1 }
}

fn main() {}

