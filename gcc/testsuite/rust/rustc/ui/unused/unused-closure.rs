// Test that closures and generators are "must use" types.
// edition:2018

#![feature(async_closure)]
#![feature(const_in_array_repeat_expressions)]
#![feature(generators)]
#![deny(unused_must_use)]

fn unused() {
    || { // { dg-error "" "" { target *-*-* } }
        println!("Hello!");
    };

    async {};    // { dg-error "" "" { target *-*-* } }
    || async {}; // { dg-error "" "" { target *-*-* } }
    async || {}; // { dg-error "" "" { target *-*-* } }


    [Box::new([|| {}; 10]); 1]; // { dg-error "" "" { target *-*-* } }

    [|| { // { dg-error "" "" { target *-*-* } }
        yield 42u32;
    }; 42];

    vec![|| "a"].pop().unwrap(); // { dg-error "" "" { target *-*-* } }

    let b = false;
        || true; // { dg-error "" "" { target *-*-* } }
    println!("{}", b);
}

fn ignored() {
    let _ = || {};
    let _ = || yield 42;
}

fn main() {
    unused();
    ignored();
}

