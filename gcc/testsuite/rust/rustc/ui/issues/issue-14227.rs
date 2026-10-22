extern {
    pub static symbol: u32;
}
static CRASH: u32 = symbol;
// { dg-error ".E0133." "" { target *-*-* } .-1 }

fn main() {}

