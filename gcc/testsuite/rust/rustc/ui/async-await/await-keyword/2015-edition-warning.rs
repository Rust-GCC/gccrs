// run-rustfix

#![allow(non_camel_case_types)]
#![deny(keyword_idents)]

mod outer_mod {
    pub mod await {
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
        pub struct await;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    }
}
use outer_mod::await::await;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }

fn main() {
    match await { await => {} }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
}

