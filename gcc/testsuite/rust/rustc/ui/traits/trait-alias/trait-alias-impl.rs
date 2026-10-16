#![feature(trait_alias)]

trait DefaultAlias = Default;

impl DefaultAlias for () {} // { dg-error ".E0404." "" { target *-*-* } }

fn main() {}

