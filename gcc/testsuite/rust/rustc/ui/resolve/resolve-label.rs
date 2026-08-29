fn f() {
    'l: loop {
        fn g() {
            loop {
                break 'l; // { dg-error ".E0767." "" { target *-*-* } }
            }
        }
    }

    loop { 'w: while break 'w { } }
}

fn main() {}

