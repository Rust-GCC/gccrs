// run-rustfix

pub foo(_s: usize) { bar() }
// { dg-error "" "" { target *-*-* } .-1 }

fn bar() {}

fn main() {
    foo(2);
}

