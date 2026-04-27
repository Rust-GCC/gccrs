// aux-build:glob-conflict.rs

extern crate glob_conflict;

fn main() {
    glob_conflict::f(); // { dg-error ".E0425." "" { target *-*-* } }
    glob_conflict::glob::f(); // { dg-error ".E0425." "" { target *-*-* } }
}

