// Here we test that the parser is able to recover in a situation like
// `for ( $pat in $expr )` since that is familiar syntax in other languages.
// Instead we suggest that the user writes `for $pat in $expr`.

#![deny(unused)] // Make sure we don't trigger `unused_parens`.

fn main() {
    let vec = vec![1, 2, 3];

    for ( elem in vec ) {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        const RECOVERY_WITNESS: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
    }
}

