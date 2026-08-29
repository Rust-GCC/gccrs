fn main() {
    let foo = "";
    let bar = "";
    format!(); // { dg-error "" "" { target *-*-* } }
    format!(struct); // { dg-error "" "" { target *-*-* } }
    format!("s", name =); // { dg-error "" "" { target *-*-* } }
    format!(
        "s {foo} {} {}",
        foo = foo,
        bar, // { dg-error "" "" { target *-*-* } }
    );
    format!("s {foo}", foo = struct); // { dg-error "" "" { target *-*-* } }
    format!("s", struct); // { dg-error "" "" { target *-*-* } }

    // This error should come after parsing errors to ensure they are non-fatal.
    format!(123); // { dg-error "" "" { target *-*-* } }
}

