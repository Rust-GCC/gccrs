// Unknown attributes fall back to unstable custom attributes.

#![feature(custom_inner_attributes)]

#![mutable_doc]
// { dg-error "" "" { target *-*-* } .-1 }

#[dance] mod a {}
// { dg-error "" "" { target *-*-* } .-1 }

#[dance] fn main() {}
// { dg-error "" "" { target *-*-* } .-1 }

