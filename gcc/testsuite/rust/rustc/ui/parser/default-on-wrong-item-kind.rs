// Test parsing for `default` where it doesn't belong.
// Specifically, we are interested in kinds of items or items in certain contexts.
// Also test item kinds in `extern` blocks and associated contexts which are not allowed there.

fn main() {}

#[cfg(FALSE)]
mod free_items {
    default extern crate foo; // { dg-error "" "" { target *-*-* } }
    default use foo; // { dg-error "" "" { target *-*-* } }
    default static foo: u8; // { dg-error "" "" { target *-*-* } }
    default const foo: u8;
    default fn foo();
    default mod foo {} // { dg-error "" "" { target *-*-* } }
    default extern "C" {} // { dg-error "" "" { target *-*-* } }
    default type foo = u8;
    default enum foo {} // { dg-error "" "" { target *-*-* } }
    default struct foo {} // { dg-error "" "" { target *-*-* } }
    default union foo {} // { dg-error "" "" { target *-*-* } }
    default trait foo {} // { dg-error "" "" { target *-*-* } }
    default trait foo = Ord; // { dg-error "" "" { target *-*-* } }
    default impl foo {}
    default!();
    default::foo::bar!();
    default default!(); // { dg-error "" "" { target *-*-* } }
    default default::foo::bar!(); // { dg-error "" "" { target *-*-* } }
    default macro foo {} // { dg-error "" "" { target *-*-* } }
    default macro_rules! foo {} // { dg-error "" "" { target *-*-* } }
}

#[cfg(FALSE)]
extern "C" {
    default extern crate foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default use foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default static foo: u8; // { dg-error "" "" { target *-*-* } }
    default const foo: u8;
// { dg-error "" "" { target *-*-* } .-1 }
    default fn foo();
    default mod foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default extern "C" {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default type foo = u8;
    default enum foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default struct foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default union foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default trait foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default trait foo = Ord; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default impl foo {}
// { dg-error "" "" { target *-*-* } .-1 }
    default!();
    default::foo::bar!();
    default default!(); // { dg-error "" "" { target *-*-* } }
    default default::foo::bar!(); // { dg-error "" "" { target *-*-* } }
    default macro foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default macro_rules! foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(FALSE)]
impl S {
    default extern crate foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default use foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default static foo: u8; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default const foo: u8;
    default fn foo();
    default mod foo {}// { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default extern "C" {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default type foo = u8;
    default enum foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default struct foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default union foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default trait foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default trait foo = Ord; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default impl foo {}
// { dg-error "" "" { target *-*-* } .-1 }
    default!();
    default::foo::bar!();
    default default!(); // { dg-error "" "" { target *-*-* } }
    default default::foo::bar!(); // { dg-error "" "" { target *-*-* } }
    default macro foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default macro_rules! foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

#[cfg(FALSE)]
trait T {
    default extern crate foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default use foo; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default static foo: u8; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default const foo: u8;
    default fn foo();
    default mod foo {}// { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default extern "C" {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default type foo = u8;
    default enum foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default struct foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default union foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default trait foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default trait foo = Ord; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default impl foo {}
// { dg-error "" "" { target *-*-* } .-1 }
    default!();
    default::foo::bar!();
    default default!(); // { dg-error "" "" { target *-*-* } }
    default default::foo::bar!(); // { dg-error "" "" { target *-*-* } }
    default macro foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    default macro_rules! foo {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

