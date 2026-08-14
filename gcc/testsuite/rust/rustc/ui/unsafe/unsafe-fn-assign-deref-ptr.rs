fn f(p: *mut u8) {
    *p = 0; // { dg-error ".E0133." "" { target *-*-* } }
    return;
}

fn main() {
}

