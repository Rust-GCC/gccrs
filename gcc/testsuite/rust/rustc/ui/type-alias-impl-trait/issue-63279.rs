// compile-flags: -Zsave-analysis

#![feature(type_alias_impl_trait)]

type Closure = impl FnOnce(); // { dg-error ".E0271." "" { target *-*-* } }

fn c() -> Closure {
    || -> Closure { || () }
}

fn main() {}

