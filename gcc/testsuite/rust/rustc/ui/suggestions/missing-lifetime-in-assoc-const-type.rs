trait ZstAssert: Sized {
    const A: &str = ""; // { dg-error ".E0106." "" { target *-*-* } }
    const B: S = S { s: &() }; // { dg-error ".E0106." "" { target *-*-* } }
    const C: &'_ str = ""; // { dg-error ".E0106." "" { target *-*-* } }
    const D: T = T { a: &(), b: &() }; // { dg-error ".E0106." "" { target *-*-* } }
}

struct S<'a> {
    s: &'a (),
}
struct T<'a, 'b> {
    a: &'a (),
    b: &'b (),
}

fn main() {}

