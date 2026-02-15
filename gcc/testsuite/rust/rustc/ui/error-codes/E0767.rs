fn main () {
    'a: loop {
        || {
            loop { break 'a; } // { dg-error ".E0767." "" { target *-*-* } }
        }
    }
}

