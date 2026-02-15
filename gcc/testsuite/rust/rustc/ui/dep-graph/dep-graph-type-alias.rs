// Test that changing what a `type` points to does not go unnoticed.

// compile-flags: -Z query-dep-graph

#![feature(rustc_attrs)]
#![allow(dead_code)]
#![allow(unused_variables)]

fn main() { }


#[rustc_if_this_changed]
type TypeAlias = u32;

// The type alias directly affects the type of the field,
// not the enclosing struct:
#[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
struct Struct {
    #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
    x: TypeAlias,
    y: u32
}

#[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
enum Enum {
    Variant1 {
        #[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
        t: TypeAlias
    },
    Variant2(i32)
}

#[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
trait Trait {
    #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
    fn method(&self, _: TypeAlias);
}

struct SomeType;

#[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
impl SomeType {
    #[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
    #[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
    fn method(&self, _: TypeAlias) {}
}

#[rustc_then_this_would_need(type_of)] // { dg-error "" "" { target *-*-* } }
type TypeAlias2 = TypeAlias;

#[rustc_then_this_would_need(fn_sig)] // { dg-error "" "" { target *-*-* } }
#[rustc_then_this_would_need(typeck)] // { dg-error "" "" { target *-*-* } }
fn function(_: TypeAlias) {

}

