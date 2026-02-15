// aux-build:lint_stability_fields.rs
#![allow(deprecated)]
#![allow(dead_code)]
#![feature(staged_api)]

#![stable(feature = "rust1", since = "1.0.0")]

mod cross_crate {
    extern crate lint_stability_fields;

    mod reexport {
        #[stable(feature = "rust1", since = "1.0.0")]
        pub use super::lint_stability_fields::*;
    }

    use self::lint_stability_fields::*;

    pub fn foo() {
        let x = Stable {
            inherit: 1,
            override1: 2, // { dg-error ".E0658." "" { target *-*-* } }
            override2: 3, // { dg-error ".E0658." "" { target *-*-* } }
        };

        let _ = x.inherit;
        let _ = x.override1; // { dg-error ".E0658." "" { target *-*-* } }
        let _ = x.override2; // { dg-error ".E0658." "" { target *-*-* } }

        let Stable {
            inherit: _,
            override1: _, // { dg-error ".E0658." "" { target *-*-* } }
            override2: _ // { dg-error ".E0658." "" { target *-*-* } }
        } = x;
        // all fine
        let Stable { .. } = x;

        let x = Stable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1; // { dg-error ".E0658." "" { target *-*-* } }
        let _ = x.2; // { dg-error ".E0658." "" { target *-*-* } }

        let Stable2(_,
                   _, // { dg-error ".E0658." "" { target *-*-* } }
                   _) // { dg-error ".E0658." "" { target *-*-* } }
            = x;
        // all fine
        let Stable2(..) = x;


        let x = Unstable { // { dg-error ".E0658." "" { target *-*-* } }
            inherit: 1, // { dg-error ".E0658." "" { target *-*-* } }
            override1: 2,
            override2: 3, // { dg-error ".E0658." "" { target *-*-* } }
        };

        let _ = x.inherit; // { dg-error ".E0658." "" { target *-*-* } }
        let _ = x.override1;
        let _ = x.override2; // { dg-error ".E0658." "" { target *-*-* } }

        let Unstable { // { dg-error ".E0658." "" { target *-*-* } }
            inherit: _, // { dg-error ".E0658." "" { target *-*-* } }
            override1: _,
            override2: _ // { dg-error ".E0658." "" { target *-*-* } }
        } = x;

        let Unstable  // { dg-error ".E0658." "" { target *-*-* } }
            // the patterns are all fine:
            { .. } = x;

        // Unstable items are still unstable even when used through a stable "pub use".
        let x = reexport::Unstable2(1, 2, 3); // { dg-error ".E0658." "" { target *-*-* } }

        let x = Unstable2(1, 2, 3); // { dg-error ".E0658." "" { target *-*-* } }

        let _ = x.0; // { dg-error ".E0658." "" { target *-*-* } }
        let _ = x.1;
        let _ = x.2; // { dg-error ".E0658." "" { target *-*-* } }

        let Unstable2  // { dg-error ".E0658." "" { target *-*-* } }
            (_, // { dg-error ".E0658." "" { target *-*-* } }
             _,
             _) // { dg-error ".E0658." "" { target *-*-* } }
            = x;
        let Unstable2 // { dg-error ".E0658." "" { target *-*-* } }
            // the patterns are all fine:
            (..) = x;


        let x = Deprecated { // { dg-error ".E0658." "" { target *-*-* } }
            inherit: 1, // { dg-error ".E0658." "" { target *-*-* } }
            override1: 2,
            override2: 3, // { dg-error ".E0658." "" { target *-*-* } }
        };

        let _ = x.inherit; // { dg-error ".E0658." "" { target *-*-* } }
        let _ = x.override1;
        let _ = x.override2; // { dg-error ".E0658." "" { target *-*-* } }

        let Deprecated { // { dg-error ".E0658." "" { target *-*-* } }
            inherit: _, // { dg-error ".E0658." "" { target *-*-* } }
            override1: _,
            override2: _ // { dg-error ".E0658." "" { target *-*-* } }
        } = x;

        let Deprecated // { dg-error ".E0658." "" { target *-*-* } }
            // the patterns are all fine:
            { .. } = x;

        let x = Deprecated2(1, 2, 3); // { dg-error ".E0658." "" { target *-*-* } }

        let _ = x.0; // { dg-error ".E0658." "" { target *-*-* } }
        let _ = x.1;
        let _ = x.2; // { dg-error ".E0658." "" { target *-*-* } }

        let Deprecated2 // { dg-error ".E0658." "" { target *-*-* } }
            (_, // { dg-error ".E0658." "" { target *-*-* } }
             _,
             _) // { dg-error ".E0658." "" { target *-*-* } }
            = x;
        let Deprecated2 // { dg-error ".E0658." "" { target *-*-* } }
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
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;

        let Stable {
            inherit: _,
            override1: _,
            override2: _
        } = x;
        // all fine
        let Stable { .. } = x;

        let x = Stable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;

        let Stable2(_,
                   _,
                   _)
            = x;
        // all fine
        let Stable2(..) = x;


        let x = Unstable {
            inherit: 1,
            override1: 2,
            override2: 3,
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;

        let Unstable {
            inherit: _,
            override1: _,
            override2: _
        } = x;

        let Unstable
            // the patterns are all fine:
            { .. } = x;


        let x = Unstable2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;

        let Unstable2
            (_,
             _,
             _)
            = x;
        let Unstable2
            // the patterns are all fine:
            (..) = x;


        let x = Deprecated {
            inherit: 1,
            override1: 2,
            override2: 3,
        };

        let _ = x.inherit;
        let _ = x.override1;
        let _ = x.override2;

        let Deprecated {
            inherit: _,
            override1: _,
            override2: _
        } = x;

        let Deprecated
            // the patterns are all fine:
            { .. } = x;

        let x = Deprecated2(1, 2, 3);

        let _ = x.0;
        let _ = x.1;
        let _ = x.2;

        let Deprecated2
            (_,
             _,
             _)
            = x;
        let Deprecated2
            // the patterns are all fine:
            (..) = x;
    }
}

fn main() {}

