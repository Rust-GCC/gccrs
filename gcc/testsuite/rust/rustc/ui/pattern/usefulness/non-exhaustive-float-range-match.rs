#![allow(illegal_floating_point_literal_pattern)]
#![deny(unreachable_patterns)]

fn main() {
    match 0.0 {
      0.0..=1.0 => {}
      _ => {} // ok
    }

    match 0.0 { // { dg-error ".E0004." "" { target *-*-* } }
      0.0..=1.0 => {}
    }
}

