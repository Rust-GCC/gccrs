// check-only
// run-rustfix

fn main() {
    match 3 {
        4 => 1,
        3 => {
            2 // { dg-error ".E0308." "" { target *-*-* } }
        }
        _ => 2
    }
    match 3 { // { dg-error ".E0308." "" { target *-*-* } }
        4 => 1,
        3 => 2,
        _ => 2
    }
    let _ = ();
}

