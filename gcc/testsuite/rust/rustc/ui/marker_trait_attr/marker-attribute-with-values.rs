#![feature(marker_trait_attr)]

#[marker(always)] // { dg-error "" "" { target *-*-* } }
trait Marker1 {}

#[marker("never")] // { dg-error "" "" { target *-*-* } }
trait Marker2 {}

#[marker(key = "value")] // { dg-error "" "" { target *-*-* } }
trait Marker3 {}

fn main() {}

