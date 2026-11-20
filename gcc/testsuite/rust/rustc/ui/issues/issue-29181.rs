// aux-build:issue-29181.rs

extern crate issue_29181 as foo;

fn main() {
    0.homura(); // { dg-error ".E0599." "" { target *-*-* } }
    // Issue #47759, detect existing method on the fundamental impl:
    let _ = |x: f64| x * 2.0.exp(); // { dg-error ".E0689." "" { target *-*-* } }
}

