#![feature(main)]

#[main]
fn main1() {
}

mod foo {
    #[main]
    fn main2() { // { dg-error ".E0137." "" { target *-*-* } }
    }
}

