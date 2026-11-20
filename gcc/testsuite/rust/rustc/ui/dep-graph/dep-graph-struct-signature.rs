// Test cases where a changing struct appears in the signature of fns
// and methods.

// compile-flags: -Z query-dep-graph

#![feature(rustc_attrs)]
#![allow(dead_code)]
#![allow(unused_variables)]

fn main() { }

#[rustc_if_this_changed]
struct WillChange {
    x: u32,
    y: u32
}

struct WontChange {
    x: u32,
    y: u32
}

// these are valid dependencies
mod signatures {
    use WillChange;

    #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
    #[rustc_then_this_would_need(associated_item)] // { dg-error "" "" { target *-*-* } }
    #[rustc_then_this_would_need(trait_def)] // { dg-error "" "" { target *-*-* } }
    trait Bar {
        #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
        fn do_something(x: WillChange);
    }

    #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
    #[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
    fn some_fn(x: WillChange) { }

    #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
    #[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
    fn new_foo(x: u32, y: u32) -> WillChange {
        WillChange { x: x, y: y }
    }

    #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
    impl WillChange {
        #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
        #[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
        fn new(x: u32, y: u32) -> WillChange { loop { } }
    }

    #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
    impl WillChange {
        #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
        #[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
        fn method(&self, x: u32) { }
    }

    struct WillChanges {
        #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
        x: WillChange,
        #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
        y: WillChange
    }

    // The fields change, not the type itself.
    #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
    fn indirect(x: WillChanges) { }
}

mod invalid_signatures {
    use WontChange;

    #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
    trait A {
        #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
        fn do_something_else_twice(x: WontChange);
    }

    #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
    fn b(x: WontChange) { }

    #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
    #[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
    fn c(x: u32) { }
}

