fn f(a: isize, b: isize) : lt(a, b) { }
// { dg-error "" "" { target *-*-* } .-1 }

fn lt(a: isize, b: isize) { }

fn main() { let a: isize = 10; let b: isize = 23; check (lt(a, b)); f(a, b); }

