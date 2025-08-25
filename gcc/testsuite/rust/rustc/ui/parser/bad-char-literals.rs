// ignore-tidy-cr
// ignore-tidy-tab

fn main() {
    // these literals are just silly.
    ''';
// { dg-error "" "" { target *-*-* } .-1 }

    // note that this is a literal "\n" byte
    '
';
// { dg-error "" "" { target *-*-* } .-2 }

    // note that this is a literal "\r" byte
    ''; // { dg-error "" "" { target *-*-* } }

    // note that this is a literal tab character here
    '	';
// { dg-error "" "" { target *-*-* } .-1 }
}

