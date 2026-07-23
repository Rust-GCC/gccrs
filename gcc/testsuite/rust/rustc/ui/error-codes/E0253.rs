mod foo {
    pub trait MyTrait {
        fn do_something();
    }
}

use foo::MyTrait::do_something;
// { dg-error ".E0253." "" { target *-*-* } .-1 }

fn main() {}

