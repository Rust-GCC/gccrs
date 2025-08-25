use self::A;
use self::B;
mod A {} // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
pub mod B {} // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
mod C {
    use C::D;
    mod D {} // { dg-error ".E0255." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

