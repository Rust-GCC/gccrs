#![feature(generators, generator_trait)]

use std::ops::Generator;

fn foo() -> impl Generator<Return = i32> { // { dg-error ".E0271." "" { target *-*-* } }
    || {
        if false {
            return Ok(6);
        }

        yield ();

        5 // { dg-error ".E0308." "" { target *-*-* } }
    }
}

fn main() {}

