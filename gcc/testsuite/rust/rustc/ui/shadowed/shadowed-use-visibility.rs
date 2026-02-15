mod foo {
    pub fn f() {}

    use foo as bar;
    pub use self::f as bar;
}

mod bar {
    use foo::bar::f as g; // { dg-error ".E0603." "" { target *-*-* } }

    use foo as f;
    pub use foo::*;
}

use bar::f::f; // { dg-error ".E0603." "" { target *-*-* } }
fn main() {}

