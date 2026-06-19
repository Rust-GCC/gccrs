fn main() {}

fn test_and() {
    let a = true;
    let b = false;

    let _ = a and b; // { dg-error "" "" { target *-*-* } }

    if a and b { // { dg-error "" "" { target *-*-* } }
        println!("both");
    }

    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

fn test_or() {
    let a = true;
    let b = false;

    let _ = a or b; // { dg-error "" "" { target *-*-* } }

    if a or b { // { dg-error "" "" { target *-*-* } }
        println!("both");
    }
}

fn test_and_par() {
    let a = true;
    let b = false;
    if (a and b) {  // { dg-error "" "" { target *-*-* } }
        println!("both");
    }
}

fn test_or_par() {
    let a = true;
    let b = false;
    if (a or b) {  // { dg-error "" "" { target *-*-* } }
        println!("both");
    }
}

fn test_while_and() {
    let a = true;
    let b = false;
    while a and b {  // { dg-error "" "" { target *-*-* } }
        println!("both");
    }
}

fn test_while_or() {
    let a = true;
    let b = false;
    while a or b { // { dg-error "" "" { target *-*-* } }
        println!("both");
    }
}

