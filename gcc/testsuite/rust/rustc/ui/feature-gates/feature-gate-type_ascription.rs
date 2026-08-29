// Type ascription is unstable

fn main() {
    let a = 10: u8; // { dg-error ".E0658." "" { target *-*-* } }
}

