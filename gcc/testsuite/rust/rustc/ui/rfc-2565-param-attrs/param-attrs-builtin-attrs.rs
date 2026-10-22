extern "C" {
    fn ffi(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    );
}

type FnType = fn(
    /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
    #[test] a: u32,
// { dg-error "" "" { target *-*-* } .-1 }
    /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
    #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
    /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
    #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
);

pub fn foo(
    /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
    #[test] a: u32,
// { dg-error "" "" { target *-*-* } .-1 }
    /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
    #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
    /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
    #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
) {}

struct SelfStruct {}
impl SelfStruct {
    fn foo(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        self,
        /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Qux
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}

    fn issue_64682_associated_fn(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Qux
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}

struct RefStruct {}
impl RefStruct {
    fn foo(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        &self,
        /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Qux
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}
trait RefTrait {
    fn foo(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        &self,
        /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Qux
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}

    fn issue_64682_associated_fn(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Qux
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}

impl RefTrait for RefStruct {
    fn foo(
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        &self,
        /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Qux
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}

fn main() {
    let _ = |
        /// Foo
// { dg-error "" "" { target *-*-* } .-1 }
        #[test] a: u32,
// { dg-error "" "" { target *-*-* } .-1 }
        /// Bar
// { dg-error "" "" { target *-*-* } .-1 }
        #[must_use]
// { dg-error "" "" { target *-*-* } .-1 }
        /// Baz
// { dg-error "" "" { target *-*-* } .-1 }
        #[no_mangle] b: i32
// { dg-error "" "" { target *-*-* } .-1 }
    | {};
}

