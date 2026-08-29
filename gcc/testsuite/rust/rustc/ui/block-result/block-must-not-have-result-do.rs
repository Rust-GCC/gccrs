fn main() {
    loop {
        true // { dg-error ".E0308." "" { target *-*-* } }
    }
}

