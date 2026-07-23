static FOO: &'static [u8] = b"\f";  // { dg-error "" "" { target *-*-* } }

pub fn main() {
    b"\f";  // { dg-error "" "" { target *-*-* } }
    b"\x0Z";  // { dg-error "" "" { target *-*-* } }
    b"é";  // { dg-error "" "" { target *-*-* } }
    b"a  // { dg-error ".E0766." "" { target *-*-* } }
}

