// Test a supertrait cycle where the first trait we find (`A`) is not
// a direct participant in the cycle.

trait A: B {
}

trait B: C {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
}

trait C: B { }

fn main() { }

