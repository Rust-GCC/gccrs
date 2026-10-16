fn main() {}
fn foo(mut s: String) -> String {
    s.push_str("asdf") // { dg-error ".E0308." "" { target *-*-* } }
}

