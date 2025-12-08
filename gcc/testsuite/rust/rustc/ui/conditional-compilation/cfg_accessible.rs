#![feature(cfg_accessible)]

mod m {
    pub struct ExistingPublic;
    struct ExistingPrivate;
}

#[cfg_accessible(m::ExistingPublic)]
struct ExistingPublic;

// FIXME: Not implemented yet.
#[cfg_accessible(m::ExistingPrivate)] // { dg-error "" "" { target *-*-* } }
struct ExistingPrivate;

// FIXME: Not implemented yet.
#[cfg_accessible(m::NonExistent)] // { dg-error "" "" { target *-*-* } }
struct ExistingPrivate;

#[cfg_accessible(n::AccessibleExpanded)] // OK, `cfg_accessible` can wait and retry.
struct AccessibleExpanded;

macro_rules! generate_accessible_expanded {
    () => {
        mod n {
            pub struct AccessibleExpanded;
        }
    };
}

generate_accessible_expanded!();

struct S {
    field: u8,
}

// FIXME: Not implemented yet.
#[cfg_accessible(S::field)] // { dg-error "" "" { target *-*-* } }
struct Field;

fn main() {
    ExistingPublic;
    AccessibleExpanded;
}

