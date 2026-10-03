use std::collections::LinkedList;

fn main() {
    LinkedList::new() += 1; // { dg-error ".E0067." "" { target *-*-* } }
// { dg-error ".E0067." "" { target *-*-* } .-1 }
}

