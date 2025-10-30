// check-fail
// compile-flags: -Z chalk

trait Foo { }

struct S<T: Foo> {
    x: T,
}

impl Foo for i32 { }
impl<T> Foo for Option<T> { }

fn main() {
    let s = S {
       x: 5,
    };

    let s = S { // { dg-error ".E0277." "" { target *-*-* } }
        x: 5.0,
    };

    let s = S {
        x: Some(5.0),
    };
}

