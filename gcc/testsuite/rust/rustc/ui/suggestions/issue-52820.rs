struct Bravery {
    guts: String,
    brains: String,
}

fn main() {
    let guts = "mettle";
    let _ = Bravery {
        guts, // { dg-error ".E0308." "" { target *-*-* } }
        brains: guts.clone(), // { dg-error ".E0308." "" { target *-*-* } }
    };
}

