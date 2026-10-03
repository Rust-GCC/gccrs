#![feature(default_type_parameter_fallback)]

fn avg<T=T::Item>(_: T) {}
// { dg-error ".E0128." "" { target *-*-* } .-1 }

fn main() {}

