enum Enum<T: Trait> {
    X = Trait::Number,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

trait Trait {
    const Number: i32 = 1;
}

fn main() {}

