#![allow(unused)]

struct PersonalityInventory {
    expressivity: f32,
    instrumentality: f32
}

impl PersonalityInventory {
    fn expressivity(&self) -> f32 {
        match *self {
            PersonalityInventory { expressivity: exp, ... } => exp
// { dg-error "" "" { target *-*-* } .-1 }
        }
    }
}

fn main() {}

