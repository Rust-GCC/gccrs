#![allow(dead_code)]

fn non_elidable<'a, 'b>(a: &'a u8, b: &'b u8) -> &'a u8 {
    a
}

// the boundaries of elision
static NON_ELIDABLE_FN: &fn(&u8, &u8) -> &u8 =
// { dg-error ".E0106." "" { target *-*-* } .-1 }
    &(non_elidable as fn(&u8, &u8) -> &u8);
// { dg-error ".E0106." "" { target *-*-* } .-1 }

fn main() {}

