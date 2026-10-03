fn main() {
    let baz = ().foo(); // { dg-error ".E0599." "" { target *-*-* } }
    <i32 as std::str::FromStr>::from_str(&baz); // No complaints about `str` being unsized
}

