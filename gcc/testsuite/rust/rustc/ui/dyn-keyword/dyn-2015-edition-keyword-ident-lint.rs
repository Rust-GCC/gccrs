// Under the 2015 edition with the keyword_idents lint, `dyn` is not
// entirely acceptable as an identifier. We currently do not attempt
// to detect or fix uses of `dyn` under a macro. Since we are testing
// this file via `rustfix`, we want the rustfix output to be
// compilable; so the macros here carefully use `dyn` "correctly."

// run-rustfix

#![allow(non_camel_case_types)]
#![deny(keyword_idents)]

mod outer_mod {
    pub mod dyn {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
        pub struct dyn;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    }
}
use outer_mod::dyn::dyn;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }

fn main() {
    match dyn { dyn => {} }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
    macro_defn::dyn();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    macro_defn::boxed();
}

mod macro_defn {
    use super::Trait;

    macro_rules! dyn {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

        // Note that we do not lint nor fix occurrences under macros
        ($dyn:tt) => { (Box<dyn Trait>, Box<$dyn Trait>) }
    }

    pub fn dyn() -> ::outer_mod::dyn::dyn {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-warning "" "" { target *-*-* } .-6 }
        ::outer_mod::dyn::dyn
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
    }



    pub fn boxed() -> dyn!(
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

            // Note that we do not lint nor fix occurrences under macros
            dyn
    )
    {
        (Box::new(1), Box::new(2))
    }
}

pub trait Trait { }

impl Trait for u32 { }

