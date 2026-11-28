// Test scope annotations from `enclosing_scope` parameter

#![feature(rustc_attrs)]

#[rustc_on_unimplemented(enclosing_scope="in this scope")]
trait Trait{}

struct Foo;

fn f<T: Trait>(x: T) {}

fn main() {
    let x = || {
        f(Foo{}); // { dg-error ".E0277." "" { target *-*-* } }
        let y = || {
            f(Foo{}); // { dg-error ".E0277." "" { target *-*-* } }
        };
    };

    {
        {
            f(Foo{}); // { dg-error ".E0277." "" { target *-*-* } }
        }
    }

    f(Foo{}); // { dg-error ".E0277." "" { target *-*-* } }
}

