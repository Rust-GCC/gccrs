// aux-build:edition-lint-paths.rs
// run-rustfix

#![feature(rust_2018_preview)]
#![deny(absolute_paths_not_starting_with_crate)]
#![allow(unused)]

extern crate edition_lint_paths;

pub mod foo {
    use edition_lint_paths;
    use ::bar::Bar;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    use super::bar::Bar2;
    use crate::bar::Bar3;

    use bar;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    use crate::{bar as something_else};

    use {Bar as SomethingElse, main};
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    use crate::{Bar as SomethingElse2, main as another_main};

    pub fn test() {
    }

    pub trait SomeTrait { }
}

use bar::Bar;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

pub mod bar {
    use edition_lint_paths as foo;
    pub struct Bar;
    pub type Bar2 = Bar;
    pub type Bar3 = Bar;
}

mod baz {
    use *;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

impl ::foo::SomeTrait for u32 { }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {
    let x = ::bar::Bar;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    let x = bar::Bar;
    let x = crate::bar::Bar;
    let x = self::bar::Bar;
    foo::test();

    {
        use edition_lint_paths as bar;
        edition_lint_paths::foo();
        bar::foo();
        ::edition_lint_paths::foo();
    }
}

