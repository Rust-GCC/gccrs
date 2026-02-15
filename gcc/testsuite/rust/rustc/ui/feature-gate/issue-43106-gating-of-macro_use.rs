// This is just a check-list of the cases where feeding arguments to
// `#[macro_use]` is rejected. (The cases where no error is emitted
// corresponds to cases where the attribute is currently unused, so we
// get that warning; see issue-43106-gating-of-builtin-attrs.rs

#![macro_use(my_macro)]
// { dg-error "" "" { target *-*-* } .-1 }

#[macro_use(my_macro)]
// { dg-error "" "" { target *-*-* } .-1 }
mod macro_escape {
    mod inner { #![macro_use(my_macro)] }
// { dg-error "" "" { target *-*-* } .-1 }

    #[macro_use = "2700"] struct S;
// { dg-error "" "" { target *-*-* } .-1 }

    #[macro_use] fn f() { }

    #[macro_use] type T = S;

    #[macro_use] impl S { }
}

fn main() { }

