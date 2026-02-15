#![feature(trait_alias)]

trait Foo {}
auto trait A = Foo; // { dg-error "" "" { target *-*-* } }
unsafe trait B = Foo; // { dg-error "" "" { target *-*-* } }

trait C: Ord = Eq; // { dg-error "" "" { target *-*-* } }
trait D: = Eq; // { dg-error "" "" { target *-*-* } }

fn main() {}

