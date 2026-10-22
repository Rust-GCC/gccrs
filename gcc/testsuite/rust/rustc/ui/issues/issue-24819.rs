use std::collections::HashSet;

fn main() {
    let mut v = Vec::new();
    foo(&mut v);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

fn foo(h: &mut HashSet<u32>) {
}

