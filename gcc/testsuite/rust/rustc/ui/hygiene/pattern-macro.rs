macro_rules! foo { () => ( x ) }

fn main() {
    let foo!() = 2;
    x + 1; // { dg-error ".E0425." "" { target *-*-* } }
}

