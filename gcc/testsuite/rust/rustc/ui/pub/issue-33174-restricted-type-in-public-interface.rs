#![allow(non_camel_case_types)] // genus is always capitalized

pub(crate) struct Snail;
// { dg-note "" "" { target *-*-* } .-1 }

mod sea {
    pub(super) struct Turtle;
// { dg-note "" "" { target *-*-* } .-1 }
}

struct Tortoise;
// { dg-note "" "" { target *-*-* } .-1 }

pub struct Shell<T> {
    pub(crate) creature: T,
}

pub type Helix_pomatia = Shell<Snail>;
// { dg-error ".E0446." "" { target *-*-* } .-1 }
// { dg-note ".E0446." "" { target *-*-* } .-2 }
pub type Dermochelys_coriacea = Shell<sea::Turtle>;
// { dg-error ".E0446." "" { target *-*-* } .-1 }
// { dg-note ".E0446." "" { target *-*-* } .-2 }
pub type Testudo_graeca = Shell<Tortoise>;
// { dg-error ".E0446." "" { target *-*-* } .-1 }
// { dg-note ".E0446." "" { target *-*-* } .-2 }

fn main() {}

