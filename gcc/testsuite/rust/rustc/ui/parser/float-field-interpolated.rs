struct S(u8, (u8, u8));

macro_rules! generate_field_accesses {
    ($a:tt, $b:literal, $c:expr) => {
        let s = S(0, (0, 0));

        s.$a; // OK
        { s.$b; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
        { s.$c; } // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
    };
}

fn main() {
    generate_field_accesses!(1.1, 1.1, 1.1);
}

