#[repr(u128)]
enum A { // { dg-error ".E0658." "" { target *-*-* } }
    A(u64)
}

fn main() {}

