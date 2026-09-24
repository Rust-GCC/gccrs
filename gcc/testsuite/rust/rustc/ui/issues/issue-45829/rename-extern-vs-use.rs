// aux-build:issue-45829-b.rs

mod foo {
    pub mod bar {}
}

use foo::bar;
extern crate issue_45829_b as bar;
// { dg-error ".E0254." "" { target *-*-* } .-1 }

fn main() {}

