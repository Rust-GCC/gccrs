pub fn main() {
    match 22 {
        0 .. 3 => {} // { dg-error ".E0658." "" { target *-*-* } }
        _ => {}
    }
}

