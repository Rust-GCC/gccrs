#![feature(generators)]

fn foo(_b: &bool, _a: ()) {}

fn main() {
    || {
        let b = true;
        foo(&b, yield); // { dg-error ".E0626." "" { target *-*-* } }
    };
}

