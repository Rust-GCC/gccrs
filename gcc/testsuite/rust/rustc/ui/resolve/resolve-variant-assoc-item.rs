enum E { V }
use E::V;

fn main() {
    E::V::associated_item; // { dg-error ".E0433." "" { target *-*-* } }
    V::associated_item; // { dg-error ".E0433." "" { target *-*-* } }
}

