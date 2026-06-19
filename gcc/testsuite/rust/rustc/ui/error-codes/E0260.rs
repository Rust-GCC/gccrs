extern crate alloc;

mod alloc {
// { dg-error ".E0260." "" { target *-*-* } .-1 }
    pub trait MyTrait {
        fn do_something();
    }
}

fn main() {}

