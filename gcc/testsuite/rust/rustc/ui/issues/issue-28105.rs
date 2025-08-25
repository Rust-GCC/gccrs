// Make sure that a continue span actually contains the keyword.

fn main() {
    continue // { dg-error ".E0268." "" { target *-*-* } }
    ;
    break // { dg-error ".E0268." "" { target *-*-* } }
    ;
}

