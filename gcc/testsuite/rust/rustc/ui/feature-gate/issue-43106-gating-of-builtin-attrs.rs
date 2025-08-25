// { dg-note "" "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// This test enumerates as many compiler-builtin ungated attributes as
// possible (that is, all the mutually compatible ones), and checks
// that we get "expected" (*) warnings for each in the various weird
// places that users might put them in the syntax.
//
// (*): The word "expected" is in quotes above because the cases where
// warnings are and are not emitted might not match a user's intuition
// nor the rustc developers' intent. I am really just trying to
// capture today's behavior in a test, not so that it become enshrined
// as the absolute behavior going forward, but rather so that we do
// not change the behavior in the future without even being *aware* of
// the change when it happens.
//
// At the time of authoring, the attributes here are listed in the
// order that they occur in `librustc_feature`.
//
// Any builtin attributes that:
//
//  - are not stable, or
//
//  - could not be included here covering the same cases as the other
//    attributes without raising an *error* from rustc (note though
//    that warnings are of course expected)
//
// have their own test case referenced by filename in an inline
// comment.
//
// The test feeds numeric inputs to each attribute that accepts them
// without error. We do this for two reasons: (1.) to exercise how
// inputs are handled by each, and (2.) to ease searching for related
// occurrences in the source text.

// check-pass
// ignore-tidy-linelength

#![feature(test, plugin_registrar)]
#![warn(unused_attributes, unknown_lints)]
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

// Exception, a gated and deprecated attribute.

#![plugin_registrar]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }

// UNGATED WHITE-LISTED BUILT-IN ATTRIBUTES

#![warn(x5400)] // { dg-warning "" "" { target *-*-* } }
#![allow(x5300)] // { dg-warning "" "" { target *-*-* } }
#![forbid(x5200)] // { dg-warning "" "" { target *-*-* } }
#![deny(x5100)] // { dg-warning "" "" { target *-*-* } }
#![macro_use] // (allowed if no argument; see issue-43160-gating-of-macro_use.rs)
// skipping testing of cfg
// skipping testing of cfg_attr
#![should_panic] // { dg-warning "" "" { target *-*-* } }
#![ignore] // { dg-warning "" "" { target *-*-* } }
#![no_implicit_prelude]
#![reexport_test_harness_main = "2900"]
// see gated-link-args.rs
// see issue-43106-gating-of-macro_escape.rs for crate-level; but non crate-level is below at "2700"
// (cannot easily test gating of crate-level #[no_std]; but non crate-level is below at "2600")
#![proc_macro_derive()] // { dg-warning "" "" { target *-*-* } }
#![doc = "2400"]
#![cold] // { dg-warning "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }
// see issue-43106-gating-of-builtin-attrs-error.rs
#![link()]
#![link_name = "1900"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
#![link_section = "1800"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// see issue-43106-gating-of-rustc_deprecated.rs
#![must_use]
// see issue-43106-gating-of-stable.rs
// see issue-43106-gating-of-unstable.rs
// see issue-43106-gating-of-deprecated.rs
#![windows_subsystem = "windows"]

// UNGATED CRATE-LEVEL BUILT-IN ATTRIBUTES

#![crate_name = "0900"]
#![crate_type = "bin"] // cannot pass "0800" here

#![crate_id = "10"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

// FIXME(#44232) we should warn that this isn't used.
#![feature(rust1)]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

#![no_start]
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

// (cannot easily gating state of crate-level #[no_main]; but non crate-level is below at "0400")
#![no_builtins]
#![recursion_limit = "0200"]
#![type_length_limit = "0100"]

// USES OF BUILT-IN ATTRIBUTES IN OTHER ("UNUSUAL") PLACES

#[warn(x5400)]
// { dg-warning "" "" { target *-*-* } .-1 }
mod warn {
    mod inner { #![warn(x5400)] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[warn(x5400)] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[warn(x5400)] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[warn(x5400)] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[warn(x5400)] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[allow(x5300)]
// { dg-warning "" "" { target *-*-* } .-1 }
mod allow {
    mod inner { #![allow(x5300)] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[allow(x5300)] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[allow(x5300)] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[allow(x5300)] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[allow(x5300)] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[forbid(x5200)]
// { dg-warning "" "" { target *-*-* } .-1 }
mod forbid {
    mod inner { #![forbid(x5200)] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[forbid(x5200)] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[forbid(x5200)] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[forbid(x5200)] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[forbid(x5200)] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[deny(x5100)]
// { dg-warning "" "" { target *-*-* } .-1 }
mod deny {
    mod inner { #![deny(x5100)] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[deny(x5100)] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[deny(x5100)] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[deny(x5100)] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[deny(x5100)] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[macro_use]
mod macro_use {
    mod inner { #![macro_use] }

    #[macro_use] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_use] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_use] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_use] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[macro_export]
// { dg-warning "" "" { target *-*-* } .-1 }
mod macro_export {
    mod inner { #![macro_export] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_export] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_export] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_export] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_export] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[plugin_registrar]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
mod plugin_registrar {
    mod inner { #![plugin_registrar] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { dg-note "" "" { target *-*-* } .-4 }

    // for `fn f()` case, see gated-plugin_registrar.rs

    #[plugin_registrar] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }

    #[plugin_registrar] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }

    #[plugin_registrar] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
}

// At time of unit test authorship, if compiling without `--test` then
// non-crate-level #[test] attributes seem to be ignored.

#[test]
mod test { mod inner { #![test] }

    fn f() { }

    struct S;

    type T = S;

    impl S { }
}

// At time of unit test authorship, if compiling without `--test` then
// non-crate-level #[bench] attributes seem to be ignored.

#[bench]
mod bench {
    mod inner { #![bench] }

    #[bench]
    struct S;

    #[bench]
    type T = S;

    #[bench]
    impl S { }
}

#[repr()]
mod repr {
    mod inner { #![repr()] }

    #[repr()] fn f() { }

    struct S;

    #[repr()] type T = S;

    #[repr()] impl S { }
}

#[path = "3800"]
mod path {
    mod inner { #![path="3800"] }

    #[path = "3800"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[path = "3800"]  struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[path = "3800"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[path = "3800"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[automatically_derived]
// { dg-warning "" "" { target *-*-* } .-1 }
mod automatically_derived {
    mod inner { #![automatically_derived] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[automatically_derived] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[automatically_derived] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[automatically_derived] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[automatically_derived] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[no_mangle]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod no_mangle {
// { dg-note "" "" { target *-*-* } .-1 }
    mod inner { #![no_mangle] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[no_mangle] fn f() { }

    #[no_mangle] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[no_mangle] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[no_mangle] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}

#[should_panic]
// { dg-warning "" "" { target *-*-* } .-1 }
mod should_panic {
    mod inner { #![should_panic] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[should_panic] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[should_panic] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[should_panic] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[should_panic] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[ignore]
// { dg-warning "" "" { target *-*-* } .-1 }
mod ignore {
    mod inner { #![ignore] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[ignore] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[ignore] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[ignore] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[ignore] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[no_implicit_prelude]
// { dg-warning "" "" { target *-*-* } .-1 }
mod no_implicit_prelude {
    mod inner { #![no_implicit_prelude] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[no_implicit_prelude] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[no_implicit_prelude] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[no_implicit_prelude] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[no_implicit_prelude] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[reexport_test_harness_main = "2900"]
// { dg-warning "" "" { target *-*-* } .-1 }
mod reexport_test_harness_main {
    mod inner { #![reexport_test_harness_main="2900"] }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[reexport_test_harness_main = "2900"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[reexport_test_harness_main = "2900"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[reexport_test_harness_main = "2900"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[reexport_test_harness_main = "2900"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

// Cannot feed "2700" to `#[macro_escape]` without signaling an error.
#[macro_escape]
// { dg-warning "" "" { target *-*-* } .-1 }
mod macro_escape {
    mod inner { #![macro_escape] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { help "" "" { target *-*-* } .-2 }

    #[macro_escape] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_escape] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_escape] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }

    #[macro_escape] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
}

#[no_std]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod no_std {
    mod inner { #![no_std] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_std] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_std] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_std] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_std] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

// At time of authorship, #[proc_macro_derive = "2500"] signals error
// when it occurs on a mod (apart from crate-level). Therefore it goes
// into its own file; see issue-43106-gating-of-proc_macro_derive.rs

#[doc = "2400"]
mod doc {
    mod inner { #![doc="2400"] }

    #[doc = "2400"] fn f() { }

    #[doc = "2400"] struct S;

    #[doc = "2400"] type T = S;

    #[doc = "2400"] impl S { }
}

#[cold]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod cold {
// { dg-note "" "" { target *-*-* } .-1 }

    mod inner { #![cold] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[cold] fn f() { }

    #[cold] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[cold] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[cold] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}

#[link_name = "1900"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod link_name {
// { dg-note "" "" { target *-*-* } .-1 }

    #[link_name = "1900"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
    extern { }
// { dg-note "" "" { target *-*-* } .-1 }

    mod inner { #![link_name="1900"] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_name = "1900"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_name = "1900"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_name = "1900"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_name = "1900"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}

#[link_section = "1800"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod link_section {
// { dg-note "" "" { target *-*-* } .-1 }

    mod inner { #![link_section="1800"] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_section = "1800"] fn f() { }

    #[link_section = "1800"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_section = "1800"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }

    #[link_section = "1800"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
}


// Note that this is a `check-pass` test, so it
// will never invoke the linker. These are here nonetheless to point
// out that we allow them at non-crate-level (though I do not know
// whether they have the same effect here as at crate-level).

#[link()]
mod link {
    mod inner { #![link()] }

    #[link()] fn f() { }

    #[link()] struct S;

    #[link()] type T = S;

    #[link()] impl S { }
}

struct StructForDeprecated;

#[deprecated]
mod deprecated {
    mod inner { #![deprecated] }

    #[deprecated] fn f() { }

    #[deprecated] struct S1;

    #[deprecated] type T = super::StructForDeprecated;

    #[deprecated] impl super::StructForDeprecated { }
}

#[must_use]
mod must_use {
    mod inner { #![must_use] }

    #[must_use] fn f() { }

    #[must_use] struct S;

    #[must_use] type T = S;

    #[must_use] impl S { }
}

#[windows_subsystem = "windows"]
mod windows_subsystem {
    mod inner { #![windows_subsystem="windows"] }

    #[windows_subsystem = "windows"] fn f() { }

    #[windows_subsystem = "windows"] struct S;

    #[windows_subsystem = "windows"] type T = S;

    #[windows_subsystem = "windows"] impl S { }
}

// BROKEN USES OF CRATE-LEVEL BUILT-IN ATTRIBUTES

#[crate_name = "0900"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod crate_name {
    mod inner { #![crate_name="0900"] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_name = "0900"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_name = "0900"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_name = "0900"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_name = "0900"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

#[crate_type = "0800"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod crate_type {
    mod inner { #![crate_type="0800"] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_type = "0800"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_type = "0800"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_type = "0800"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[crate_type = "0800"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

#[feature(x0600)]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod feature {
    mod inner { #![feature(x0600)] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[feature(x0600)] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[feature(x0600)] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[feature(x0600)] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[feature(x0600)] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}


#[no_main]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod no_main_1 {
    mod inner { #![no_main] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_main] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_main] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_main] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[no_main] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

#[no_builtins]
mod no_builtins {
    mod inner { #![no_builtins] }

    #[no_builtins] fn f() { }

    #[no_builtins] struct S;

    #[no_builtins] type T = S;

    #[no_builtins] impl S { }
}

#[recursion_limit="0200"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod recursion_limit {
    mod inner { #![recursion_limit="0200"] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[recursion_limit="0200"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[recursion_limit="0200"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[recursion_limit="0200"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[recursion_limit="0200"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

#[type_length_limit="0100"]
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
mod type_length_limit {
    mod inner { #![type_length_limit="0100"] }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[type_length_limit="0100"] fn f() { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[type_length_limit="0100"] struct S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[type_length_limit="0100"] type T = S;
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

    #[type_length_limit="0100"] impl S { }
// { dg-warning "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

fn main() {}

