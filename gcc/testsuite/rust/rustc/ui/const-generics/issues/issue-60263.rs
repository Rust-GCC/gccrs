struct B<const I: u8>; // { dg-error ".E0658." "" { target *-*-* } }

impl B<0> {
    fn bug() -> Self {
        panic!()
    }
}

fn main() {}

