fn main() {
    '\u{_10FFFF}'; // { dg-error "" "" { target *-*-* } }
}

