fn main() {
    [1; || {}]; // { dg-error ".E0308." "" { target *-*-* } }
}

