#![feature(intrinsics)]

#[lang = "sized"]
pub trait Sized {}

extern "rust-intrinsic" {
    pub fn ctpop<T>(x: T) -> u32;
}

fn main() -> i32 {
    let pop1 = ctpop(42i32) - 3;
    let pop2 = ctpop(42u32) - 3;
    let pop3 = ctpop(42i64) - 3;
    let pop4 = ctpop(42u64) - 3;
    let pop5 = ctpop(-42i32) - 29;
    let pop6 = ctpop(0x1234567812345678i64) - 26;
    let pop7 = ctpop(-1i64) - 64;
    (pop1 + pop2 + pop3 + pop4 + pop5 + pop6 + pop7) as i32
}
