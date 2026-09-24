#![feature(const_indexing)]

const ARR: [i32; 6] = [42, 43, 44, 45, 46, 47];
const IDX: usize = 3;
const VAL: i32 = ARR[IDX];
const BONG: [i32; (ARR[0] - 41) as usize] = [5];
const BLUB: [i32; (ARR[0] - 40) as usize] = [5];
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const BOO: [i32; (ARR[0] - 41) as usize] = [5, 99];
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }

fn main() {
    let _ = VAL;
}

