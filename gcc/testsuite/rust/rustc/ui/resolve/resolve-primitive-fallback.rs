fn main() {
    // Make sure primitive type fallback doesn't work in value namespace
    std::mem::size_of(u16);
// { dg-error ".E0061." "" { target *-*-* } .-1 }
// { dg-error ".E0061." "" { target *-*-* } .-2 }

    // Make sure primitive type fallback doesn't work with global paths
    let _: ::u8;
// { dg-error ".E0412." "" { target *-*-* } .-1 }
}

