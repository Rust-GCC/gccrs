// This test checks cases where the derive-macro does not exist.

mod derive {
    #[derive(x3300)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    union U { f: i32 }

    #[derive(x3300)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    enum E { }

    #[derive(x3300)]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    struct S;
}

fn main() {}

