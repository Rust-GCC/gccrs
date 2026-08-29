extern
    "C"suffix // { dg-error "" "" { target *-*-* } }
    fn foo() {}

extern
    "C"suffix // { dg-error "" "" { target *-*-* } }
{}

fn main() {
    ""suffix; // { dg-error "" "" { target *-*-* } }
    b""suffix; // { dg-error "" "" { target *-*-* } }
    r#""#suffix; // { dg-error "" "" { target *-*-* } }
    br#""#suffix; // { dg-error "" "" { target *-*-* } }
    'a'suffix; // { dg-error "" "" { target *-*-* } }
    b'a'suffix; // { dg-error "" "" { target *-*-* } }

    1234u1024; // { dg-error "" "" { target *-*-* } }
    1234i1024; // { dg-error "" "" { target *-*-* } }
    1234f1024; // { dg-error "" "" { target *-*-* } }
    1234.5f1024; // { dg-error "" "" { target *-*-* } }

    1234suffix; // { dg-error "" "" { target *-*-* } }
    0b101suffix; // { dg-error "" "" { target *-*-* } }
    1.0suffix; // { dg-error "" "" { target *-*-* } }
    1.0e10suffix; // { dg-error "" "" { target *-*-* } }
}

