fn main() {
    let x = 0u8;
    x as Vec<u8>; // { dg-error ".E0605." "" { target *-*-* } }

    let v = std::ptr::null::<u8>();
    v as &u8; // { dg-error ".E0605." "" { target *-*-* } }
}

