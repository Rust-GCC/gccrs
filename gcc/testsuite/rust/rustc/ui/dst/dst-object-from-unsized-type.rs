// Test that we cannot create objects from unsized types.

trait Foo { fn foo(&self) {} }
impl Foo for str {}
impl Foo for [u8] {}

fn test1<T: ?Sized + Foo>(t: &T) {
    let u: &dyn Foo = t;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn test2<T: ?Sized + Foo>(t: &T) {
    let v: &dyn Foo = t as &dyn Foo;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn test3() {
    let _: &[&dyn Foo] = &["hi"];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn test4(x: &[u8]) {
    let _: &dyn Foo = x as &dyn Foo;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() { }

