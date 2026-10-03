// check that the local data keys are private by default.

mod bar {
    thread_local!(static baz: f64 = 0.0);
}

fn main() {
    bar::baz.with(|_| ());
// { dg-error ".E0603." "" { target *-*-* } .-1 }
}

