fn main() {
    'aaaaab: loop {
        || {
            loop { continue 'aaaaaa }
// { dg-error ".E0426." "" { target *-*-* } .-1 }
        };

    }
}

