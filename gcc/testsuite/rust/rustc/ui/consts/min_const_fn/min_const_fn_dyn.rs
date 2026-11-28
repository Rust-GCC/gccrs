struct HasDyn {
    field: &'static dyn std::fmt::Debug,
}

struct Hide(HasDyn);

const fn no_inner_dyn_trait(_x: Hide) {}
const fn no_inner_dyn_trait2(x: Hide) {
    x.0.field;
// { dg-error ".E0723." "" { target *-*-* } .-1 }
}
const fn no_inner_dyn_trait_ret() -> Hide { Hide(HasDyn { field: &0 }) }
// { dg-error ".E0723." "" { target *-*-* } .-1 }

fn main() {}

