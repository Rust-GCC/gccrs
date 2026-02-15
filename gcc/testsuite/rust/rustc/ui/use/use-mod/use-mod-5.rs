mod foo {
    pub mod bar {
        pub fn drop() {}
    }
}

use foo::bar::self;
// { dg-error ".E0429." "" { target *-*-* } .-1 }

fn main() {
    // Because of error recovery this shouldn't error
    bar::drop();
}

