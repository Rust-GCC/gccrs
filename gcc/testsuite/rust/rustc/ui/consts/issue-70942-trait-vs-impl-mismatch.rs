trait Nat {
    const VALUE: usize;
}

struct Zero;

impl Nat for Zero {
    const VALUE: i32 = 0;
// { dg-error ".E0326." "" { target *-*-* } .-1 }
}

fn main() {
    let _: [i32; Zero::VALUE] = [];
}

