fn main() {}

type X<'a> = (?'a) +;
// { dg-error ".E0224." "" { target *-*-* } .-1 }
// { dg-error ".E0224." "" { target *-*-* } .-2 }
// { dg-warning ".E0224." "" { target *-*-* } .-3 }

