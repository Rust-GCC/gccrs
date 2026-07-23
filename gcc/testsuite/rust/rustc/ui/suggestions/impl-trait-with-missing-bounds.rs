// The double space in `impl  Iterator` is load bearing! We want to make sure we don't regress by
// accident if the internal string representation changes.
#[rustfmt::skip]
fn foo(constraints: impl  Iterator) {
    for constraint in constraints {
        qux(constraint);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

fn bar<T>(t: T, constraints: impl Iterator) where T: std::fmt::Debug {
    for constraint in constraints {
        qux(t);
        qux(constraint);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

fn baz(t: impl std::fmt::Debug, constraints: impl Iterator) {
    for constraint in constraints {
        qux(t);
        qux(constraint);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

fn bat<I, T: std::fmt::Debug>(t: T, constraints: impl Iterator, _: I) {
    for constraint in constraints {
        qux(t);
        qux(constraint);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

fn bak(constraints: impl  Iterator + std::fmt::Debug) {
    for constraint in constraints {
        qux(constraint);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    }
}

fn qux(_: impl std::fmt::Debug) {}

fn main() {}

