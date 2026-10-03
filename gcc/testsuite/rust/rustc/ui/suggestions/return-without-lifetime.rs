struct Thing<'a>(&'a ());
struct Foo<'a>(&usize);
// { dg-error ".E0106." "" { target *-*-* } .-1 }

fn func1<'a>(_arg: &'a Thing) -> &() { unimplemented!() }
// { dg-error ".E0106." "" { target *-*-* } .-1 }
fn func2<'a>(_arg: &Thing<'a>) -> &() { unimplemented!() }
// { dg-error ".E0106." "" { target *-*-* } .-1 }

fn main() {}

