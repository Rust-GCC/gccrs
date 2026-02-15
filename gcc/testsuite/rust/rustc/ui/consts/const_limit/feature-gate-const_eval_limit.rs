#![const_eval_limit="42"]
// { dg-error ".E0658." "" { target *-*-* } .-1 }

const CONSTANT: usize = limit();

fn main() {
    assert_eq!(CONSTANT, 1764);
}

const fn limit() -> usize {
    let x = 42;

    x * 42
}

