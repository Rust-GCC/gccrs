#![feature(generators)]

fn main() {
    let x = (|_| {},);

    || {
        let x = x;

        x.0({ // { dg-error ".E0626." "" { target *-*-* } }
            yield;
        });
    };
}

