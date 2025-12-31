fn main() {
    let x = 1u8;
    match x {
        0u8..=3i8 => (), // { dg-error ".E0308." "" { target *-*-* } }
        _ => ()
    }
}

