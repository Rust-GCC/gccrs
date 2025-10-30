// aux-build:lint_stability_fields.rs

#![deny(deprecated)]
#![allow(dead_code)]
#![feature(staged_api, unstable_test_feature)]

#![stable(feature = "rust1", since = "1.0.0")]

mod cross_crate {
    extern crate lint_stability_fields;

    use self::lint_stability_fields::*;

    pub fn foo() {
        let x = Stable {
            inherit: 1,
            override1: 2,
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable {
            inherit: _,
            override1: _,
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;
        // all fine
        let Stable { .. } = x;

        let x = Stable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable2(_,
                   _,
                   _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        // all fine
        let Stable2(..) = x;


        let x = Unstable {
            inherit: 1,
            override1: 2,
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Unstable {
            inherit: _,
            override1: _,
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;

        let Unstable
            // the patterns are all fine:
            { .. } = x;


        let x = Unstable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Unstable2
            (_,
             _,
             _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        let Unstable2
            // the patterns are all fine:
            (..) = x;


        let x = Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: 1,
// { dg-error "" "" { target *-*-* } .-1 }
            override1: 2,
// { dg-error "" "" { target *-*-* } .-1 }
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.override1;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: _,
// { dg-error "" "" { target *-*-* } .-1 }
            override1: _,
// { dg-error "" "" { target *-*-* } .-1 }
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;

        let Deprecated
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            { .. } = x;

        let x = Deprecated2(1, 2, 3);
// { dg-error "" "" { target *-*-* } .-1 }

        let _ = x.0;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.1;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            (_,
// { dg-error "" "" { target *-*-* } .-1 }
             _,
// { dg-error "" "" { target *-*-* } .-1 }
             _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            (..) = x;
    }
}

mod this_crate {
    #[stable(feature = "rust1", since = "1.0.0")]
    struct Stable {
        inherit: u8,
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        override1: u8,
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        override2: u8,
    }

    #[stable(feature = "rust1", since = "1.0.0")]
    struct Stable2(u8,
                   #[stable(feature = "rust1", since = "1.0.0")] u8,
                   #[unstable(feature = "unstable_test_feature", issue = "none")]
                   #[rustc_deprecated(since = "1.0.0", reason = "text")] u8);

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    struct Unstable {
        inherit: u8,
        #[stable(feature = "rust1", since = "1.0.0")]
        override1: u8,
        #[rustc_deprecated(since = "1.0.0", reason = "text")]
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        override2: u8,
    }

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    struct Unstable2(u8,
                     #[stable(feature = "rust1", since = "1.0.0")] u8,
                     #[unstable(feature = "unstable_test_feature", issue = "none")]
                     #[rustc_deprecated(since = "1.0.0", reason = "text")] u8);

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    struct Deprecated {
        inherit: u8,
        #[stable(feature = "rust1", since = "1.0.0")]
        override1: u8,
        #[unstable(feature = "unstable_test_feature", issue = "none")]
        override2: u8,
    }

    #[unstable(feature = "unstable_test_feature", issue = "none")]
    #[rustc_deprecated(since = "1.0.0", reason = "text")]
    struct Deprecated2(u8,
                       #[stable(feature = "rust1", since = "1.0.0")] u8,
                       #[unstable(feature = "unstable_test_feature", issue = "none")] u8);

    pub fn foo() {
        let x = Stable {
            inherit: 1,
            override1: 2,
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable {
            inherit: _,
            override1: _,
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;
        // all fine
        let Stable { .. } = x;

        let x = Stable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Stable2(_,
                   _,
                   _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        // all fine
        let Stable2(..) = x;


        let x = Unstable {
            inherit: 1,
            override1: 2,
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Unstable {
            inherit: _,
            override1: _,
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;

        let Unstable
            // the patterns are all fine:
            { .. } = x;


        let x = Unstable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Unstable2
            (_,
             _,
             _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        let Unstable2
            // the patterns are all fine:
            (..) = x;


        let x = Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: 1,
// { dg-error "" "" { target *-*-* } .-1 }
            override1: 2,
// { dg-error "" "" { target *-*-* } .-1 }
            override2: 3,
// { dg-error "" "" { target *-*-* } .-1 }
        };

        let _ = x.inherit;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.override1;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.override2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated {
// { dg-error "" "" { target *-*-* } .-1 }
            inherit: _,
// { dg-error "" "" { target *-*-* } .-1 }
            override1: _,
// { dg-error "" "" { target *-*-* } .-1 }
            override2: _
// { dg-error "" "" { target *-*-* } .-1 }
        } = x;

        let Deprecated
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            { .. } = x;

        let x = Deprecated2(1, 2, 3);
// { dg-error "" "" { target *-*-* } .-1 }

        let _ = x.0;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.1;
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = x.2;
// { dg-error "" "" { target *-*-* } .-1 }

        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            (_,
// { dg-error "" "" { target *-*-* } .-1 }
             _,
// { dg-error "" "" { target *-*-* } .-1 }
             _)
// { dg-error "" "" { target *-*-* } .-1 }
            = x;
        let Deprecated2
// { dg-error "" "" { target *-*-* } .-1 }
            // the patterns are all fine:
            (..) = x;
    }
}

fn main() {}

