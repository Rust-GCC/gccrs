// normalize-stderr-test "pref: Align \{\n *pow2: [1-3],\n *\}" -> "pref: $$PREF_ALIGN"
#![feature(never_type, rustc_attrs, type_alias_impl_trait)]
#![crate_type = "lib"]

#[rustc_layout(debug)]
enum E { Foo, Bar(!, i32, i32) } // { dg-error "" "" { target *-*-* } }

#[rustc_layout(debug)]
struct S { f1: i32, f2: (), f3: i32 } // { dg-error "" "" { target *-*-* } }

#[rustc_layout(debug)]
union U { f1: (i32, i32), f3: i32 } // { dg-error "" "" { target *-*-* } }

#[rustc_layout(debug)]
type Test = Result<i32, i32>; // { dg-error "" "" { target *-*-* } }

#[rustc_layout(debug)]
type T = impl std::fmt::Debug; // { dg-error "" "" { target *-*-* } }

fn f() -> T {
    0i32
}

