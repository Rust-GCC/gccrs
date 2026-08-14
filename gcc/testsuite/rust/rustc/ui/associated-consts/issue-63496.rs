trait A {
    const C: usize;

    fn f() -> ([u8; A::C], [u8; A::C]);
// { dg-error ".E0283." "" { target *-*-* } .-1 }
// { dg-error ".E0283." "" { target *-*-* } .-2 }
}

fn main() {}

