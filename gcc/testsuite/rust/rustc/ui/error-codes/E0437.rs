trait Foo {}

impl Foo for i32 {
    type Bar = bool; // { dg-error ".E0437." "" { target *-*-* } }
}

fn main () {
}

