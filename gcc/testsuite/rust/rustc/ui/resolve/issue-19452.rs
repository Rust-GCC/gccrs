// aux-build:issue-19452-aux.rs

extern crate issue_19452_aux;

enum Homura {
    Madoka { age: u32 }
}

fn main() {
    let homura = Homura::Madoka;
// { dg-error ".E0423." "" { target *-*-* } .-1 }

    let homura = issue_19452_aux::Homura::Madoka;
// { dg-error ".E0423." "" { target *-*-* } .-1 }
}

