#![feature(discriminant_kind)]

use std::marker::DiscriminantKind;

enum Uninhabited {}

struct NewType;

impl DiscriminantKind for NewType {
// { dg-error ".E0322." "" { target *-*-* } .-1 }
    type Discriminant = Uninhabited;
}

fn main() {}

