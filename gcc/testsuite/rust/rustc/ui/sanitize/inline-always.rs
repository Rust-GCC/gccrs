// check-pass

#![feature(no_sanitize)]

#[inline(always)]
// { dg-note "" "" { target *-*-* } .-1 }
#[no_sanitize(address)]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
fn x() {
}

fn main() {
    x()
}

