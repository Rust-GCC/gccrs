fn f(p: *const u8) -> u8 {
    return *p; // { dg-error ".E0133." "" { target *-*-* } }
}

fn main() {
}

