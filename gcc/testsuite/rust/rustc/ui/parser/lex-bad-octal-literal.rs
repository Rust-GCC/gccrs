fn main() {
    0o18; // { dg-error "" "" { target *-*-* } }
    0o1234_9_5670;  // { dg-error "" "" { target *-*-* } }
}

