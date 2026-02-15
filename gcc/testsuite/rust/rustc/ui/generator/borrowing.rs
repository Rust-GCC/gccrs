#![feature(generators, generator_trait)]

use std::ops::Generator;
use std::pin::Pin;

fn main() {
    let _b = {
        let a = 3;
        Pin::new(&mut || yield &a).resume(())
// { dg-error ".E0597." "" { target *-*-* } .-1 }
    };

    let _b = {
        let a = 3;
        || {
            yield &a
// { dg-error ".E0597." "" { target *-*-* } .-1 }
        }
    };
}

