// compile-flags: -Z chalk

trait Foo: Sized { }

trait Bar {
    type Item: Foo;
}

impl Foo for i32 { }

impl Foo for str { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }


// Implicit `T: Sized` bound.
impl<T> Foo for Option<T> { }

trait Baz<U: ?Sized> where U: Foo { }

impl Baz<i32> for i32 { }

impl Baz<f32> for f32 { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() {
}

