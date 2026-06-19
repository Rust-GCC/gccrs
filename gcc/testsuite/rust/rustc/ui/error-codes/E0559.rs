enum Field {
    Fool { x: u32 },
}

fn main() {
    let s = Field::Fool { joke: 0 };
// { dg-error ".E0559." "" { target *-*-* } .-1 }
}

