// compile-flags: -Zverbose

#![feature(generators, generator_trait)]

fn main() {
    let x = "Type mismatch test";
    let generator :() = || {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        yield 1i32;
        return x
    };
}

