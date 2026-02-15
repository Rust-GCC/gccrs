macro_rules! define_struct {
    ($t:ty) => {
        struct S1(pub($t));
        struct S2(pub (in foo) ());
        struct S3(pub($t) ());
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

mod foo {
    define_struct! { foo } // { dg-error ".E0412." "" { target *-*-* } }
// { dg-error ".E0412." "" { target *-*-* } .-2 }
}

fn main() {}

