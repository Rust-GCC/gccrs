fn main() {}

fn f() -> isize { fn f() -> isize {} pub f<
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } }

