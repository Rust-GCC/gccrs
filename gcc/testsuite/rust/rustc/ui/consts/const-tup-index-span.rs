// Test spans of errors

const TUP: (usize,) = 5usize << 64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
const ARR: [i32; TUP.0] = [];

fn main() {
}

