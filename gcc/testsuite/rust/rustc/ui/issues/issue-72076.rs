trait X {
    type S;
    fn f() -> Self::S {} // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

