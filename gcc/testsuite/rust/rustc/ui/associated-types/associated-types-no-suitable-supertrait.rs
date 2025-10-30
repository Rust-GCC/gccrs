// Check that we get an error when you use `<Self as Get>::Value` in
// the trait definition but `Self` does not, in fact, implement `Get`.
//
// See also associated-types-no-suitable-supertrait-2.rs, which checks
// that we see the same error if we get around to checking the default
// method body.
//
// See also run-pass/associated-types-projection-to-unrelated-trait.rs,
// which checks that the trait interface itself is not considered an
// error as long as all impls satisfy the constraint.

trait Get {
    type Value;
}

trait Other {
    fn uhoh<U:Get>(&self, foo: U, bar: <Self as Get>::Value) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

impl<T:Get> Other for T {
    fn uhoh<U:Get>(&self, foo: U, bar: <(T, U) as Get>::Value) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() { }

