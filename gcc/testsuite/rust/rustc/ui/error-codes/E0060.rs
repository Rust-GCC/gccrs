extern "C" {
    fn printf(_: *const u8, ...) -> u32;
}

fn main() {
    unsafe { printf(); }
// { dg-error ".E0060." "" { target *-*-* } .-1 }
// { dg-error ".E0060." "" { target *-*-* } .-2 }
}

