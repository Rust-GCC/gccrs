// Proc macros using `mixed_site` spans exhibit usual properties of `macro_rules` hygiene.

// aux-build:mixed-site-span.rs

#[macro_use]
extern crate mixed_site_span;

struct ItemUse;

fn main() {
    'label_use: loop {
        let local_use = 1;
        proc_macro_rules!();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
        ItemDef; // OK
        local_def; // { dg-error ".E0425." "" { target *-*-* } }
    }
}

macro_rules! pass_dollar_crate {
    () => (proc_macro_rules!($crate);) // { dg-error ".E0412." "" { target *-*-* } }
}
pass_dollar_crate!();

