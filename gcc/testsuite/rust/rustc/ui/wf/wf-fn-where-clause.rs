// Test that we check where-clauses on fn items.


#![allow(dead_code)]

trait ExtraCopy<T:Copy> { }

fn foo<T,U>() where T: ExtraCopy<U> // { dg-error ".E0277." "" { target *-*-* } }
{
}

fn bar() where Vec<dyn Copy>:, {}
// { dg-error ".E0038." "" { target *-*-* } .-1 }
// { dg-error ".E0038." "" { target *-*-* } .-2 }

struct Vec<T> {
    t: T,
}

fn main() { }

