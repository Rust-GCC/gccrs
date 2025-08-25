// Test that we give suitable error messages when the user attempts to
// impl a trait `Trait` for its own object type.

trait Foo { fn dummy(&self) { } }
trait Bar: Foo { }
trait Baz: Bar { }

// Supertraits of Baz are not legal:
impl Foo for dyn Baz { }
// { dg-error ".E0371." "" { target *-*-* } .-1 }
impl Bar for dyn Baz { }
// { dg-error ".E0371." "" { target *-*-* } .-1 }
impl Baz for dyn Baz { }
// { dg-error ".E0371." "" { target *-*-* } .-1 }

// But other random traits are:
trait Other { }
impl Other for dyn Baz { } // OK, Other not a supertrait of Baz

fn main() { }

