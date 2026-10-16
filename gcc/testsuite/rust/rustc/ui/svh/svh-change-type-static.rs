// ignore-msvc FIXME #31306

// note that these aux-build directives must be in this order
// aux-build:svh-a-base.rs
// aux-build:svh-b.rs
// aux-build:svh-a-change-type-static.rs
// normalize-stderr-test: "(crate `(\w+)`:) .*" -> "$1 $$PATH_$2"

extern crate a;
extern crate b; // { dg-error ".E0460." "" { target *-*-* } }

fn main() {
    b::foo()
}

