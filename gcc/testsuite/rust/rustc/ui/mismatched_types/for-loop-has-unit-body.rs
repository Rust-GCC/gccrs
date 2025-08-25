fn main() {
    for x in 0..3 {
        x // { dg-error ".E0308." "" { target *-*-* } }
    }
}

