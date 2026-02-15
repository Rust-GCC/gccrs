// `macro_rules` items produced by transparent macros have correct hygiene in basic cases.
// Local variables and labels are hygienic, items are not hygienic.
// `$crate` refers to the crate that defines `macro_rules` and not the outer transparent macro.

// aux-build:gen-macro-rules-hygiene.rs

#[macro_use]
extern crate gen_macro_rules_hygiene;

struct ItemUse;

gen_macro_rules!();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }

fn main() {
    'label_use: loop {
        let local_use = 1;
        generated!();
        ItemDef; // OK
        local_def; // { dg-error ".E0425." "" { target *-*-* } }
    }
}

