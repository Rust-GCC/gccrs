fn main() {
    'a: loop {
        || {
            loop { continue 'a }
// { dg-error ".E0767." "" { target *-*-* } .-1 }
        };

    }
}

