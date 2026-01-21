// build-fail

#![warn(const_err)]

const X: u32 = 5;
const Y: u32 = 6;
const FOO: u32 = [X - Y, Y - X][(X < Y) as usize];
// { dg-warning "" "" { target *-*-* } .-1 }

fn main() {
    println!("{}", FOO);
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-warning ".E0080." "" { target *-*-* } .-2 }
}

