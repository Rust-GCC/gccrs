trait A {
    type Bla;
    fn to_bla(&self) -> Bla;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
}

fn main() {}

