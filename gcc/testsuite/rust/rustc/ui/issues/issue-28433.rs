enum Bird {
    pub Duck,
// { dg-error ".E0449." "" { target *-*-* } .-1 }
    Goose,
    pub(crate) Dove
// { dg-error ".E0449." "" { target *-*-* } .-1 }
}


fn main() {
    let y = Bird::Goose;
}

