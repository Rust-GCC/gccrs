// At time of authorship, #[proc_macro_derive = "2500"] will emit an
// error when it occurs on a mod (apart from crate-level), but will
// not descend further into the mod for other occurrences of the same
// error.
//
// This file sits on its own because the "weird" occurrences here
// signal errors, making it incompatible with the "warnings only"
// nature of issue-43106-gating-of-builtin-attrs.rs

#[proc_macro_derive()]
// { dg-error "" "" { target *-*-* } .-1 }
mod proc_macro_derive1 {
    mod inner { #![proc_macro_derive()] }
    // (no error issued here if there was one on outer module)
}

mod proc_macro_derive2 {
    mod inner { #![proc_macro_derive()] }
// { dg-error "" "" { target *-*-* } .-1 }

    #[proc_macro_derive()] fn f() { }
// { dg-error "" "" { target *-*-* } .-1 }

    #[proc_macro_derive()] struct S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[proc_macro_derive()] type T = S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[proc_macro_derive()] impl S { }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

