// Macro from prelude is shadowed by non-existent import recovered as `Res::Err`.

mod m {}
use m::assert; // { dg-error ".E0432." "" { target *-*-* } }

fn main() {
    assert!(true);
}

