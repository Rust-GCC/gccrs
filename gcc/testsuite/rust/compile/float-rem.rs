#![feature(no_core)]
#![no_core]

// { dg-final { scan-tree-dump "remainder" "gimple" } }

// so that nothing gets const folded

fn two_something() -> f32 {
    2.9
}

fn four_something() -> f32 {
    4.07
}

fn main() {
    let _ = four_something() % two_something();
}
