fn equal<T>(a: &T, b: &T) -> bool where T : Eq { a == b }

struct Struct;

fn main() {
    drop(equal(&Struct, &Struct))
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

