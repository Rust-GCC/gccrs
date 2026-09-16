pub trait A: Copy {}

pub trait D {
    fn f<T>(self)
        where T<Bogus = Self::AlsoBogus>: A;
// { dg-error ".E0229." "" { target *-*-* } .-1 }
}

fn main() {}

