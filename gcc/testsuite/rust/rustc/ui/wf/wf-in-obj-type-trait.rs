// Check that we enforce WF conditions also for types in fns.

trait Object<T> { }

struct MustBeCopy<T:Copy> {
    t: T
}

struct Bar<T> {
    // needs T: Copy
    x: dyn Object<MustBeCopy<T>> // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() { }

