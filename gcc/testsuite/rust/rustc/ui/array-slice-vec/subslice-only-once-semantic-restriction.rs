fn main() {
    let a: &[u8] = &[];
    match a {
        [1, tail @ .., tail @ ..] => {},
// { dg-error ".E0416." "" { target *-*-* } .-1 }
// { dg-error ".E0416." "" { target *-*-* } .-2 }
        _ => ()
    }
}

const RECOVERY_WITNESS: () = 0; // { dg-error ".E0308." "" { target *-*-* } }

