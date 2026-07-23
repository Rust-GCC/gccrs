struct A<T>(T);

impl A<bool> {
    const B: A<u8> = Self(0);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn main() {}

