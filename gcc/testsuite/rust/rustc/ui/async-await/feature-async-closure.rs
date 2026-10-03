// edition:2018
// gate-test-async_closure

fn f() {
    let _ = async || {}; // { dg-error ".E0658." "" { target *-*-* } }
}

fn main() {}

