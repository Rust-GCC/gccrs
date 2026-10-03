fn main() {
    assert_eq!(10u64, 10usize); // { dg-error ".E0308." "" { target *-*-* } }
}

