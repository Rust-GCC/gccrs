type A0 = dyn;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
type A1 = dyn::dyn;
// { dg-error ".E0433." "" { target *-*-* } .-1 }
type A2 = dyn<dyn, dyn>;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
// { dg-error ".E0412." "" { target *-*-* } .-2 }
// { dg-error ".E0412." "" { target *-*-* } .-3 }
type A3 = dyn<<dyn as dyn>::dyn>;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
// { dg-error ".E0412." "" { target *-*-* } .-2 }
// { dg-error ".E0412." "" { target *-*-* } .-3 }

fn main() {}

