pub fn main() {
    let x = () + (); // { dg-error ".E0369." "" { target *-*-* } }

    // this shouldn't have a flow-on error:
    for _ in x {}
}

