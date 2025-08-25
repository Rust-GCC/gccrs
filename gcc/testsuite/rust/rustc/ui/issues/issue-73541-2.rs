// edition:2018

async fn c() {
    'a: loop {
        macro_rules! b {
            () => {
                continue 'a
// { dg-error ".E0767." "" { target *-*-* } .-1 }
            }
        }

        async {
            loop {
                b!();
            }
        };
    }
}

fn main() { }

