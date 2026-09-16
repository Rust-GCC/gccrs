// edition:2018

fn main() {
    'a: loop {
        async {
            loop {
                continue 'a
// { dg-error ".E0767." "" { target *-*-* } .-1 }
            }
        };
    }
}

