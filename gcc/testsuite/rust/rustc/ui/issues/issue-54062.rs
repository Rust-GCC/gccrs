use std::sync::Mutex;

struct Test {
    comps: Mutex<String>,
}

fn main() {}

fn testing(test: Test) {
    let _ = test.comps.inner.try_lock();
// { dg-error ".E0616." "" { target *-*-* } .-1 }
}

