fn main() {
    concat!(b'f');  // { dg-error "" "" { target *-*-* } }
    concat!(b"foo");  // { dg-error "" "" { target *-*-* } }
    concat!(foo);   // { dg-error "" "" { target *-*-* } }
    concat!(foo()); // { dg-error "" "" { target *-*-* } }
}

