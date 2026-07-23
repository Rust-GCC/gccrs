fn f<F:Nonexist(isize) -> isize>(x: F) {}
// { dg-error ".E0405." "" { target *-*-* } .-1 }

type Typedef = isize;

fn g<F:Typedef(isize) -> isize>(x: F) {}
// { dg-error ".E0404." "" { target *-*-* } .-1 }

fn main() {}

