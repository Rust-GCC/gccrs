#[derive(Clone)]
union U {
    a: u8
}

#[derive(Clone)]
union W {
    a: std::mem::ManuallyDrop<String>
}

impl Copy for U {} // OK
impl Copy for W {} // { dg-error ".E0204." "" { target *-*-* } }

fn main() {}

