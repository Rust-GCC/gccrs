// aux-build:issue-59764.rs
// compile-flags:--extern issue_59764
// edition:2018

#![allow(warnings)]

// This tests the suggestion to import macros from the root of a crate. This aims to capture
// the case where a user attempts to import a macro from the definition location instead of the
// root of the crate and the macro is annotated with `#![macro_export]`.

// Edge cases..

mod multiple_imports_same_line_at_end {
    use issue_59764::foo::{baz, makro};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod multiple_imports_multiline_at_end_trailing_comma {
    use issue_59764::foo::{
        baz,
        makro, // { dg-error ".E0432." "" { target *-*-* } }
    };
}

mod multiple_imports_multiline_at_end {
    use issue_59764::foo::{
        baz,
        makro // { dg-error ".E0432." "" { target *-*-* } }
    };
}

mod multiple_imports_same_line_in_middle {
    use issue_59764::foo::{baz, makro, foobar};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod multiple_imports_multiline_in_middle_trailing_comma {
    use issue_59764::foo::{
        baz,
        makro, // { dg-error ".E0432." "" { target *-*-* } }
        foobar,
    };
}

mod multiple_imports_multiline_in_middle {
    use issue_59764::foo::{
        baz,
        makro, // { dg-error ".E0432." "" { target *-*-* } }
        foobar
    };
}

mod nested_imports {
    use issue_59764::{foobaz, foo::makro};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod nested_multiple_imports {
    use issue_59764::{foobaz, foo::{baz, makro}};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod nested_multiline_multiple_imports_trailing_comma {
    use issue_59764::{
        foobaz,
        foo::{
            baz,
            makro, // { dg-error ".E0432." "" { target *-*-* } }
        },
    };
}

mod nested_multiline_multiple_imports {
    use issue_59764::{
        foobaz,
        foo::{
            baz,
            makro // { dg-error ".E0432." "" { target *-*-* } }
        }
    };
}

mod doubly_nested_multiple_imports {
    use issue_59764::{foobaz, foo::{baz, makro, barbaz::{barfoo}}};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod doubly_multiline_nested_multiple_imports {
    use issue_59764::{
        foobaz,
        foo::{
            baz,
            makro, // { dg-error ".E0432." "" { target *-*-* } }
            barbaz::{
                barfoo,
            }
        }
    };
}

mod renamed_import {
    use issue_59764::foo::makro as baz;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod renamed_multiple_imports {
    use issue_59764::foo::{baz, makro as foobar};
// { dg-error ".E0432." "" { target *-*-* } .-1 }
}

mod lots_of_whitespace {
    use
        issue_59764::{

            foobaz,


            foo::{baz,

                makro as foobar} // { dg-error ".E0432." "" { target *-*-* } }

        };
}

// Simple case..

use issue_59764::foo::makro;
// { dg-error ".E0432." "" { target *-*-* } .-1 }

makro!(bar);
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    bar();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
}

