#![feature(trait_alias)]

trait EqAlias = Eq;
trait IteratorAlias = Iterator;

fn main() {
    let _: &dyn EqAlias = &123;
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    let _: &dyn IteratorAlias = &vec![123].into_iter();
// { dg-error ".E0191." "" { target *-*-* } .-1 }
}

