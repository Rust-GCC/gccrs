// Check that imports with nakes super and self don't fail during parsing
// FIXME: this shouldn't fail during name resolution either

mod a {
    mod b {
        use self as A;
// { dg-error ".E0429." "" { target *-*-* } .-1 }
        use super as B;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { dg-error ".E0432." "" { target *-*-* } .-2 }
        use super::{self as C};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { dg-error ".E0432." "" { target *-*-* } .-2 }
    }
}

fn main() {}

