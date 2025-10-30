// Test that parentheses form parses in expression paths.

struct Bar<A,R> {
    f: A, r: R
}

impl<A,B> Bar<A,B> {
    fn new() -> Bar<A,B> { panic!() }
}

fn bar() {
    let b = Bar::<isize, usize>::new(); // OK

    let b = Bar::(isize, usize)::new(); // OK too (for the parser)
// { dg-error ".E0214." "" { target *-*-* } .-1 }
}

fn main() {}

