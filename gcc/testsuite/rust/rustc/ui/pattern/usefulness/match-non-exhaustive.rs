fn main() {
    match 0 { 1 => () } // { dg-error ".E0004." "" { target *-*-* } }
    match 0 { 0 if false => () } // { dg-error ".E0004." "" { target *-*-* } }
}

