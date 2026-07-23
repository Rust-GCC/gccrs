// Issue #33262

pub fn main() {
    for i in 0..a as { }
// { dg-error "" "" { target *-*-* } .-1 }
}

