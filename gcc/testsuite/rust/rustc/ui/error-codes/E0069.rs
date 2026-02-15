fn foo() -> u8 {
    return;
// { dg-error ".E0069." "" { target *-*-* } .-1 }
}

fn main() {
}

