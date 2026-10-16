mod glob_ok {
    pub mod something {
        pub mod something_else {}
    }
}

mod single_err {}

use glob_ok::*; // glob_ok::something
use single_err::something; // { dg-error ".E0432." "" { target *-*-* } }
use something::something_else;

fn main() {}

