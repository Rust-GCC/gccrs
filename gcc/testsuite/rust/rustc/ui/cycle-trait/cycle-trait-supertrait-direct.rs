// Test a supertrait cycle where a trait extends itself.

trait Chromosome: Chromosome {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
}

fn main() { }

