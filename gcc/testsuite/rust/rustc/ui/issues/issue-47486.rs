fn main() {
    () < std::mem::size_of::<_>(); // { dg-error ".E0308." "" { target *-*-* } }
    [0u8; std::mem::size_of::<_>()]; // { dg-error ".E0282." "" { target *-*-* } }
}

