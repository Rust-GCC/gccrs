mod foo {
    extern crate core;
}

// Check that private crates can be used from outside their modules, albeit with warnings
use foo::core::cell; // { dg-error ".E0603." "" { target *-*-* } }

fn f() {
    foo::core::cell::Cell::new(0); // { dg-error ".E0603." "" { target *-*-* } }

    use foo::*;
    mod core {} // Check that private crates are not glob imported
}

mod bar {
    pub extern crate core;
}

mod baz {
    pub use bar::*;
    use self::core::cell; // Check that public extern crates are glob imported
}

fn main() {}

