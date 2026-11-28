fn main() {
    match 1i32 {
        1i32 => 1,
        2u32 => 1, // { dg-error ".E0308." "" { target *-*-* } }
        _ => 2,
    };
}

