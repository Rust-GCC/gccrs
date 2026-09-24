// build-fail
// ignore-tidy-linelength
// revisions: legacy v0
//[legacy]compile-flags: -Z symbol-mangling-version=legacy
    //[v0]compile-flags: -Z symbol-mangling-version=v0

#![feature(rustc_attrs)]

// This test is the same code as in ui/issue-53912.rs but this test checks that the symbol mangling
// fix produces the correct result, whereas that test just checks that the reproduction compiles
// successfully and doesn't crash LLVM

fn dummy() {}

mod llvm {
    pub(crate) struct Foo;
}
mod foo {
    pub(crate) struct Foo<T>(T);

    impl Foo<::llvm::Foo> {
        #[rustc_symbol_name]
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
        pub(crate) fn foo() {
            for _ in 0..0 {
                for _ in &[::dummy()] {
                    ::dummy();
                    ::dummy();
                    ::dummy();
                }
            }
        }
    }

    pub(crate) fn foo() {
        Foo::foo();
        Foo::foo();
    }
}

pub fn foo() {
    foo::foo();
}

fn main() {}

