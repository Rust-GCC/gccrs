// aux-build:crate_a1.rs
// aux-build:crate_a2.rs

// Issue 22750
// This tests the extra help message reported when a trait bound
// is not met but the struct implements a trait with the same path.

fn main() {
    let foo = {
        extern crate crate_a2 as a;
        a::Foo
    };

    let implements_no_traits = {
        extern crate crate_a2 as a;
        a::DoesNotImplementTrait
    };

    let other_variant_implements_mismatched_trait = {
        extern crate crate_a2 as a;
        a::ImplementsWrongTraitConditionally { _marker: std::marker::PhantomData::<isize> }
    };

    let other_variant_implements_correct_trait = {
        extern crate crate_a1 as a;
        a::ImplementsTraitForUsize { _marker: std::marker::PhantomData::<isize> }
    };

    {
        extern crate crate_a1 as a;
        a::try_foo(foo);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }

        // We don't want to see the "version mismatch" help message here
        // because `implements_no_traits` has no impl for `Foo`
        a::try_foo(implements_no_traits);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

        // We don't want to see the "version mismatch" help message here
        // because `other_variant_implements_mismatched_trait`
        // does not have an impl for its `<isize>` variant,
        // only for its `<usize>` variant.
        a::try_foo(other_variant_implements_mismatched_trait);
// { dg-error ".E0277." "" { target *-*-* } .-1 }

        // We don't want to see the "version mismatch" help message here
        // because `ImplementsTraitForUsize` only has
        // impls for the correct trait where the path is not misleading.
        a::try_foo(other_variant_implements_correct_trait);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
    }
}

