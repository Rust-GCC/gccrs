fn foo() -> bool {
    b"".starts_with(stringify!(foo))
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

