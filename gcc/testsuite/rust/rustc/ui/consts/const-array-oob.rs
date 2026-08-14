#![feature(const_indexing)]

const FOO: [usize; 3] = [1, 2, 3];
const BAR: usize = FOO[5]; // no error, because the error below occurs before regular const eval

const BLUB: [u32; FOO[4]] = [5, 6];
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }

fn main() {
    let _ = BAR;
}

