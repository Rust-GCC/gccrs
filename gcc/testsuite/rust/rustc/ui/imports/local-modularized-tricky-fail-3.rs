// Crate-local macro expanded `macro_export` macros cannot be accessed with module-relative paths.

macro_rules! define_exported { () => {
    #[macro_export]
    macro_rules! exported {
        () => ()
    }
}}

define_exported!();

mod m {
    use exported;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {
    ::exported!();
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

