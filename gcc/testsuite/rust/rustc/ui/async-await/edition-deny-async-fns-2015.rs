// edition:2015

async fn foo() {} // { dg-error ".E0670." "" { target *-*-* } }

fn baz() { async fn foo() {} } // { dg-error ".E0670." "" { target *-*-* } }

async fn async_baz() { // { dg-error ".E0670." "" { target *-*-* } }
    async fn bar() {} // { dg-error ".E0670." "" { target *-*-* } }
}

struct Foo {}

impl Foo {
    async fn foo() {} // { dg-error ".E0670." "" { target *-*-* } }
}

trait Bar {
    async fn foo() {} // { dg-error ".E0706." "" { target *-*-* } }
// { dg-error ".E0706." "" { target *-*-* } .-1 }
}

fn main() {
    macro_rules! accept_item { ($x:item) => {} }

    accept_item! {
        async fn foo() {} // { dg-error ".E0670." "" { target *-*-* } }
    }

    accept_item! {
        impl Foo {
            async fn bar() {} // { dg-error ".E0670." "" { target *-*-* } }
        }
    }

    let inside_closure = || {
        async fn bar() {} // { dg-error ".E0670." "" { target *-*-* } }
    };
}

