fn main() {
    let _: u32 = i32::from(0_u8); // { dg-error ".E0308." "" { target *-*-* } }
}

