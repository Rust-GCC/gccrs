fn main() {
    fn f(a: [u8; u32::DOESNOTEXIST]) {}
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

