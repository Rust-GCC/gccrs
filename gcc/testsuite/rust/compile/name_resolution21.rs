pub mod foo {
    pub macro bar() {}
}

use foo::bar;
use foo::bar; // { dg-error ".bar. defined multiple times" }

fn main() {
    bar!();
}
