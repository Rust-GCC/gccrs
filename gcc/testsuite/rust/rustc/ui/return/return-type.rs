struct S<T> {
    t: T,
}

fn foo<T>(x: T) -> S<T> {
    S { t: x }
}

fn bar() {
    foo(4 as usize)
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

