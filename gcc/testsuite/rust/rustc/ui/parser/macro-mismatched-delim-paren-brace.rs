fn main() {
    foo! (
        bar, "baz", 1, 2.0
    } // { dg-error "" "" { target *-*-* } }
} // { dg-error "" "" { target *-*-* } }

