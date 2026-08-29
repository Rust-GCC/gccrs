fn main() {
    let b: &[u8] = include_str!("file.txt");    // { dg-error ".E0308." "" { target *-*-* } }
    let s: &str = include_bytes!("file.txt");   // { dg-error ".E0308." "" { target *-*-* } }
}

