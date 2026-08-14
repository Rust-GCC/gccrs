fn with_closure<F, A>(_: F)
    where F: FnOnce(A, A)
{
}

fn a() {
    with_closure(|x: u32, y| {
        // We deduce type of `y` from `x`.
    });
}

fn b() {
    // Here we take the supplied types, resulting in an error later on.
    with_closure(|x: u32, y: i32| {
// { dg-error ".E0631." "" { target *-*-* } .-1 }
    });
}

fn c() {
    with_closure(|x, y: i32| {
        // We deduce type of `x` from `y`.
    });
}

fn main() { }

