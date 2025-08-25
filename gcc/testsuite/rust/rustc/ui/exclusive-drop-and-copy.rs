// issue #20126

#[derive(Copy, Clone)] // { dg-error ".E0184." "" { target *-*-* } }
struct Foo;

impl Drop for Foo {
    fn drop(&mut self) {}
}

#[derive(Copy, Clone)] // { dg-error ".E0184." "" { target *-*-* } }
struct Bar<T>(::std::marker::PhantomData<T>);

impl<T> Drop for Bar<T> {
    fn drop(&mut self) {}
}

fn main() {}

