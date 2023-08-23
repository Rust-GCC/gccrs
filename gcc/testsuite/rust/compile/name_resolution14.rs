pub mod foo {
    pub macro bar() {}
}

use foo::biz; // { dg-error "unresolved import .foo::biz." }

use foo::{bar, baz, biz};
// { dg-error "unresolved import .foo::baz." "" { target *-*-* } .-1 }
// { dg-error "unresolved import .foo::biz." "" { target *-*-* } .-2 }

fn main() {
    bar!();
}
