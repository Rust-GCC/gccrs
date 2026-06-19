struct Foo<'a>(&'a u8);
struct Baz<'a>(&'_ &'a u8); // { dg-error ".E0106." "" { target *-*-* } }

fn foo<'_> // { dg-error ".E0637." "" { target *-*-* } }
(_: Foo<'_>) {}

trait Meh<'a> {}
impl<'a> Meh<'a> for u8 {}

fn meh() -> Box<dyn for<'_> Meh<'_>> // { dg-error ".E0106." "" { target *-*-* } }
// { dg-error ".E0106." "" { target *-*-* } .-1 }
{
  Box::new(5u8)
}

fn foo2(_: &'_ u8, y: &'_ u8) -> &'_ u8 { y } // { dg-error ".E0106." "" { target *-*-* } }

fn main() {
    let x = 5;
    foo(Foo(&x));
    let _ = meh();
}

