fn cast_thin_to_fat(x: *const ()) {
    x as *const [u8];
// { dg-error ".E0607." "" { target *-*-* } .-1 }
}

fn main() {}

