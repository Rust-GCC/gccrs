//! Checks that associated type defaults are properly validated.
//!
//! This means:
//! * Default types are checked against where clauses on the assoc. type
//!   (eg. `type Assoc: Clone = NotClone`)

#![feature(associated_type_defaults)]

struct NotClone;

// Assoc. type bounds must hold for the default type
trait Tr {
    type Ty: Clone = NotClone;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

// Where-clauses defined on the trait must also be considered
trait Tr2
where
    Self::Ty: Clone,
{
    type Ty = NotClone;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

// Involved type parameters must fulfill all bounds required by defaults that mention them
trait Foo<T> {
    type Bar: Clone = Vec<T>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

trait Bar: Sized {
    // `(): Foo<Self>` might hold for some possible impls but not all.
    type Assoc: Foo<Self> = ();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

trait IsU8<T> {}
impl<T> IsU8<u8> for T {}

// Test that mentioning the assoc. type inside where clauses is not allowed
trait C where
    Vec<Self::Assoc>: Clone,
    Self::Assoc: IsU8<Self::Assoc>,
    bool: IsU8<Self::Assoc>,
{
    type Assoc = u8;
}

// Test that we get all expected errors if that default is unsuitable
trait D where
    Vec<Self::Assoc>: Clone,
    Self::Assoc: IsU8<Self::Assoc>,
    bool: IsU8<Self::Assoc>,
{
    type Assoc = NotClone;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

// Test behavior of the check when defaults refer to other defaults:

// Shallow substitution rejects this trait since `Baz` isn't guaranteed to be
// `Clone`.
trait Foo2<T> {
    type Bar: Clone = Vec<Self::Baz>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    type Baz = T;
}

// Adding a `T: Clone` bound doesn't help since the requirement doesn't see `T`
// because of the shallow substitution. If we did a deep substitution instead,
// this would be accepted.
trait Foo25<T: Clone> {
    type Bar: Clone = Vec<Self::Baz>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    type Baz = T;
}

// Adding the `Baz: Clone` bound isn't enough since the default is type
// parameter `T`, which also might not be `Clone`.
trait Foo3<T>
where
    Self::Bar: Clone,
    Self::Baz: Clone,
{
    type Bar = Vec<Self::Baz>;
    type Baz = T;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

// This one finally works, with `Clone` bounds on all assoc. types and the type
// parameter.
trait Foo4<T>
where
    T: Clone,
{
    type Bar: Clone = Vec<Self::Baz>;
    type Baz: Clone = T;
}

fn main() {}

