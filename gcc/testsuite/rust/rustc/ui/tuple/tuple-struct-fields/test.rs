mod foo {
    type T = ();
    struct S1(pub(in foo) (), pub(T), pub(crate) (), pub(((), T)));
    struct S2(pub((foo)) ());
// { dg-error ".E0412." "" { target *-*-* } .-1 }
// { dg-error ".E0412." "" { target *-*-* } .-2 }
}

fn main() {}

