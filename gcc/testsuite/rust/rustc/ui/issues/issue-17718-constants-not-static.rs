fn id<T>(x: T) -> T { x }

const FOO: usize = 3;

fn foo() -> &'static usize { &id(FOO) }
// { dg-error ".E0515." "" { target *-*-* } .-1 }

fn main() {
}

