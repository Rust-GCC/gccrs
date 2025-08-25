// build-fail
// ignore-tidy-linelength
// revisions: legacy v0
//[legacy]compile-flags: -Z symbol-mangling-version=legacy
    //[v0]compile-flags: -Z symbol-mangling-version=v0
//[legacy]normalize-stderr-32bit: "hee444285569b39c2" -> "SYMBOL_HASH"
//[legacy]normalize-stderr-64bit: "h310ea0259fc3d32d" -> "SYMBOL_HASH"

#![feature(optin_builtin_traits, rustc_attrs)]
#![allow(dead_code)]

mod foo {
    pub struct Foo { x: u32 }

    impl Foo {
        #[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
        #[rustc_def_path]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        fn bar() { }
    }
}

mod bar {
    use foo::Foo;

    impl Foo {
        #[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
        #[rustc_def_path]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        fn baz() { }
    }
}

trait Foo {
    type Assoc;
}

auto trait AutoTrait {}

fn main() {
    // Test closure mangling, and disambiguators.
    || {};
    || {
        trait Bar {
            fn method(&self) {}
        }

        // Test type mangling, by putting them in an `impl` header.
        impl Bar for [&'_ (dyn Foo<Assoc = extern fn(&u8, ...)> + AutoTrait); 3] {
            #[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
            #[rustc_def_path]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
            fn method(&self) {}
        }
    };
}

