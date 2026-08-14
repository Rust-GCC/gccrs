fn main() {
    format!(); // { dg-error "" "" { target *-*-* } }
    format!("" 1); // { dg-error "" "" { target *-*-* } }
    format!("", 1 1); // { dg-error "" "" { target *-*-* } }
}

