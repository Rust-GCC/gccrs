// Check that we enforce WF conditions also for types in fns.

struct MustBeCopy<T:Copy> {
    t: T
}

struct Foo<T> {
    // needs T: 'static
    x: fn() -> MustBeCopy<T> // { dg-error ".E0277." "" { target *-*-* } }
}

fn main() { }

