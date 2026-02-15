// aux-build:issue-41652-b.rs

extern crate issue_41652_b;

struct S;

impl issue_41652_b::Tr for S {
    fn f() {
        3.f()
// { dg-error ".E0689." "" { target *-*-* } .-1 }
    }
}

fn main() {}

