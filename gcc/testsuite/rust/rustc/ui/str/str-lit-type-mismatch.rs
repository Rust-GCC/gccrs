fn main() {
    let x: &[u8] = "foo"; // { dg-error ".E0308." "" { target *-*-* } }
    let y: &[u8; 4] = "baaa"; // { dg-error ".E0308." "" { target *-*-* } }
    let z: &str = b"foo"; // { dg-error ".E0308." "" { target *-*-* } }
}

