trait Tr : Sized {
    fn test<X>(u: X) -> Self {
        u   // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn main() {}

