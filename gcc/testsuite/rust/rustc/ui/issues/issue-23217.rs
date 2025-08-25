pub enum SomeEnum {
    B = SomeEnum::A, // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() {}

