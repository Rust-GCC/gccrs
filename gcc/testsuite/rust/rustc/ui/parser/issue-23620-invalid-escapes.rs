fn main() {
    let _ = b"\u{a66e}";
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = b'\u{a66e}';
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = b'\u';
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = b'\x5';
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = b'\xxy';
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = '\x5';
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = '\xxy';
// { dg-error "" "" { target *-*-* } .-1 }

    let _ = b"\u{a4a4} \xf \u";
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

    let _ = "\xf \u";
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

    let _ = "\u8f";
// { dg-error "" "" { target *-*-* } .-1 }
}

