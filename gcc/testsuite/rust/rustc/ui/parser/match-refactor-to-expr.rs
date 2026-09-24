// run-rustfix

fn main() {
    let foo =
        match // { dg-note "" "" { target *-*-* } }
        Some(4).unwrap_or(5)
// { dg-note "" "" { target *-*-* } .-1 }
        ; // { dg-note "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }

    println!("{}", foo)
}

