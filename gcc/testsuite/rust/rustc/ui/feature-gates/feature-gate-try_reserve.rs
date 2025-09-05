fn main() {
    let v = Vec::new();
    v.try_reserve(10); // { dg-error ".E0658." "" { target *-*-* } }
}

