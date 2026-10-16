// Test that attempts to implicitly coerce a value into an
// object respect the lifetime bound on the object type.

trait Foo {}
impl<'a> Foo for &'a [u8] {}

fn a(v: &[u8]) -> Box<dyn Foo + 'static> {
    let x: Box<dyn Foo + 'static> = Box::new(v); // { dg-error ".E0759." "" { target *-*-* } }
    x
}

fn b(v: &[u8]) -> Box<dyn Foo + 'static> {
    Box::new(v) // { dg-error ".E0759." "" { target *-*-* } }
}

fn c(v: &[u8]) -> Box<dyn Foo> {
    // same as previous case due to RFC 599

    Box::new(v) // { dg-error ".E0759." "" { target *-*-* } }
}

fn d<'a,'b>(v: &'a [u8]) -> Box<dyn Foo+'b> {
    Box::new(v) // { dg-error ".E0495." "" { target *-*-* } }
}

fn e<'a:'b,'b>(v: &'a [u8]) -> Box<dyn Foo+'b> {
    Box::new(v) // OK, thanks to 'a:'b
}

fn main() { }

