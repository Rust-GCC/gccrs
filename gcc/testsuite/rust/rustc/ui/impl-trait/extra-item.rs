// aux-build:extra-item.rs
// compile-flags:--extern extra_item

struct S;

impl extra_item::MyTrait for S {
    fn extra() {} // { dg-error ".E0407." "" { target *-*-* } }
}

fn main() {}

