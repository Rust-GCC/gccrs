mod a {
    pub mod b1 {
        pub enum C2 {}
    }

    pub enum B2 {}
}

use a::{b1::{C1, C2}, B2};
// { dg-error ".E0432." "" { target *-*-* } .-1 }

fn main() {
    let _: C2;
    let _: B2;
}

