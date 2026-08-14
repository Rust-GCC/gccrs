trait Foo {
    type T;
    fn foo(&self, t: Self::T);
// { dg-note "" "" { target *-*-* } .-1 }
}

impl Foo for u32 {
    type T = ();

    fn foo(&self, t: impl Clone) {}
// { dg-error ".E0049." "" { target *-*-* } .-1 }
// { dg-note ".E0049." "" { target *-*-* } .-2 }
// { dg-note ".E0049." "" { target *-*-* } .-3 }
}

fn main() {}

