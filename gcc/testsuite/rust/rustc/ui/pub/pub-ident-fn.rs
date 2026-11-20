// run-rustfix

pub   foo(_s: usize) -> bool { true }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    foo(2);
}

