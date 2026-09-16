// compile-flags: -Z parse-only

struct S<
    T: 'a + Tr, // OK
    T: Tr + 'a, // OK
    T: 'a, // OK
    T:, // OK
    T: ?for<'a> Trait, // OK
    T: Tr +, // OK
    T: ?'a, // { dg-error "" "" { target *-*-* } }

    T: ?const Tr, // OK
    T: ?const ?Tr, // OK
    T: ?const Tr + 'a, // OK
    T: ?const 'a, // { dg-error "" "" { target *-*-* } }
>;

fn main() {}

