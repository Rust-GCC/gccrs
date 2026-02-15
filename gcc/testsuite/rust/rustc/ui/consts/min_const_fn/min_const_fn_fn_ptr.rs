// gate-test-const_fn_fn_ptr_basics

struct HasPtr {
    field: fn(),
}

struct Hide(HasPtr);

fn field() {}

const fn no_inner_dyn_trait(_x: Hide) {}
const fn no_inner_dyn_trait2(x: Hide) {
    x.0.field;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}
const fn no_inner_dyn_trait_ret() -> Hide { Hide(HasPtr { field }) }
// { dg-error ".E0658." "" { target *-*-* } .-1 }

fn main() {}

