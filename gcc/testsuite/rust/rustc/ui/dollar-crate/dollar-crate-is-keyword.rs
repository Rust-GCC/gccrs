macro_rules! m {
    () => {
        // Avoid having more than one `$crate`-named item in the same module,
        // as even though they error, they still parse as `$crate` and conflict.
        mod foo {
            struct $crate {} // { dg-error "" "" { target *-*-* } }
        }

        use $crate; // { dg-error "" "" { target *-*-* } }
        use $crate as $crate; // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

m!();

fn main() {}

