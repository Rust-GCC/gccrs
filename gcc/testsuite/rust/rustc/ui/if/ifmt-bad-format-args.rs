fn main() {
    format_args!(); // { dg-error "" "" { target *-*-* } }
    format_args!(|| {}); // { dg-error "" "" { target *-*-* } }
}

