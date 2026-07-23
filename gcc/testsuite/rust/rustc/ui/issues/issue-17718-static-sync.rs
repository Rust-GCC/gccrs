#![feature(negative_impls)]

use std::marker::Sync;

struct Foo;
impl !Sync for Foo {}

static FOO: usize = 3;
static BAR: Foo = Foo;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {}

