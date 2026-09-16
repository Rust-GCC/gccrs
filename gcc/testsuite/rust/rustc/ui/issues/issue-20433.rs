fn main() {}

struct The;

impl The {
    fn iceman(c: Vec<[i32]>) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

