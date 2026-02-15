#![feature(bindings_after_at)]

fn main() {
    let mut is_mut @ not_mut = 42;
    &mut is_mut;
    &mut not_mut;
// { dg-error ".E0596." "" { target *-*-* } .-1 }

    let not_mut @ mut is_mut = 42;
    &mut is_mut;
    &mut not_mut;
// { dg-error ".E0596." "" { target *-*-* } .-1 }
}

