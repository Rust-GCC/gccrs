#![allow(dead_code, unused_variables, non_camel_case_types, non_upper_case_globals)]
#![deny(keyword_idents)]

// edition:2015
// run-rustfix

fn async() {} // { dg-error "" "" { target *-*-* } }
// { dg-warning "" "" { target *-*-* } .-1 }

macro_rules! foo {
    ($foo:ident) => {};
    ($async:expr, async) => {};
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-warning "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
}

foo!(async);
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

mod dont_lint_raw {
    fn r#async() {}
}

mod async_trait {
    trait async {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    struct MyStruct;
    impl async for MyStruct {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

mod async_static {
    static async: u32 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

mod async_const {
    const async: u32 = 0;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

struct Foo;
impl Foo { fn async() {} }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }

fn main() {
    struct async {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
    let async: async = async {};
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-warning "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-5 }
// { dg-warning "" "" { target *-*-* } .-6 }
}

#[macro_export]
macro_rules! produces_async {
    () => (pub fn async() {})
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

#[macro_export]
macro_rules! consumes_async {
    (async) => (1)
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-warning "" "" { target *-*-* } .-2 }
}

