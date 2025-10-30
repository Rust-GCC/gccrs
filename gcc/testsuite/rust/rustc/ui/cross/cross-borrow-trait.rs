// Test that cross-borrowing (implicitly converting from `Box<T>` to `&T`) is
// forbidden when `T` is a trait.

struct Foo;
trait Trait { fn foo(&self) {} }
impl Trait for Foo {}

pub fn main() {
    let x: Box<dyn Trait> = Box::new(Foo);
    let _y: &dyn Trait = x; // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
}

