// aux-build:issue-30123-aux.rs

extern crate issue_30123_aux;
use issue_30123_aux::*;

fn main() {
    let ug = Graph::<i32, i32>::new_undirected();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

