//https://github.com/rust-lang/rust/issues/31364

const fn a() -> usize {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
    b()
}
const fn b() -> usize {
    a()
}
const ARR: [i32; a()] = [5; 6];

fn main() {}

