#![feature(decl_macro)]
#![deny(unused_macros)]

// Most simple case
macro unused { // { dg-error "" "" { target *-*-* } }
    () => {}
}

#[allow(unused_macros)]
mod bar {
    // Test that putting the #[deny] close to the macro's definition
    // works.

    #[deny(unused_macros)]
    macro unused { // { dg-error "" "" { target *-*-* } }
        () => {}
    }
}

mod boo {
    pub(crate) macro unused { // { dg-error "" "" { target *-*-* } }
        () => {}
    }
}

fn main() {}

