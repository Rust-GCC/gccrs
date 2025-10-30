// Test that impl trait does not allow creating recursive types that are
// otherwise forbidden. Even when there's an opaque type in another crate
// hiding this.

fn id<T>(t: T) -> impl Sized { t }

fn recursive_id() -> impl Sized { // { dg-error ".E0720." "" { target *-*-* } }
    id(recursive_id2())
}

fn recursive_id2() -> impl Sized { // { dg-error ".E0720." "" { target *-*-* } }
    id(recursive_id())
}

fn wrap<T>(t: T) -> impl Sized { (t,) }

fn recursive_wrap() -> impl Sized { // { dg-error ".E0720." "" { target *-*-* } }
    wrap(recursive_wrap2())
}

fn recursive_wrap2() -> impl Sized { // { dg-error ".E0720." "" { target *-*-* } }
    wrap(recursive_wrap())
}

fn main() {}

