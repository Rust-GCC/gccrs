fn foo() -> bool {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    break true; // { dg-error ".E0268." "" { target *-*-* } }
}

fn main() {
    break {}; // { dg-error ".E0268." "" { target *-*-* } }
}

