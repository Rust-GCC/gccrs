// compile-flags: --cfg something
// edition:2018

#![feature(async_closure)]
#![deny(unused_variables)]

async fn foo_async(
    a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    #[allow(unused_variables)] b: i32,
) {}
fn foo(
    #[allow(unused_variables)] a: i32,
    b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
) {}

struct RefStruct {}
impl RefStruct {
    async fn bar_async(
        &self,
        a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        #[allow(unused_variables)] b: i32,
    ) {}
    fn bar(
        &self,
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
    fn issue_64682_associated_fn(
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}
trait RefTrait {
    fn bar(
        &self,
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
    fn issue_64682_associated_fn(
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}
impl RefTrait for RefStruct {
    fn bar(
        &self,
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
    fn issue_64682_associated_fn(
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    ) {}
}

fn main() {
    let _: fn(_, _) = foo;
    let a = async move |
        a: i32,
// { dg-error "" "" { target *-*-* } .-1 }
        #[allow(unused_variables)] b: i32,
    | {};
    let b = |
        #[allow(unused_variables)] a: i32,
        b: i32,
// { dg-error "" "" { target *-*-* } .-1 }
    | {};
    let _ = a(1, 2);
    let _ = b(1, 2);
}

