// aux-build:use-from-trait-xc.rs

extern crate use_from_trait_xc;
pub use use_from_trait_xc::Trait;

fn main() {
    match () {
        Trait { x: 42 } => () // { dg-error ".E0574." "" { target *-*-* } }
    }
}

