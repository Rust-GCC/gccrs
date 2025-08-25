// Check that we enforce WF conditions also for return types in fn items.

#![feature(rustc_attrs)]
#![allow(dead_code)]

struct MustBeCopy<T:Copy> {
    t: T
}

fn bar<T>() -> MustBeCopy<T> // { dg-error ".E0277." "" { target *-*-* } }
{
}

fn main() { }

