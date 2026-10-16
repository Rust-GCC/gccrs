mod foo {
    pub mod bar {
        pub fn drop() {}
    }
}

use foo::bar::self as abc;
// { dg-error ".E0429." "" { target *-*-* } .-1 }

fn main() {
    // Because of error recovery this shouldn't error
    abc::drop();
}

