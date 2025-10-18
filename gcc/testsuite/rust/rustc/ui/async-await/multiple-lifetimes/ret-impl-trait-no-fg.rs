// edition:2018

// Test that a feature gate is needed to use `impl Trait` as the
// return type of an async.

trait Trait<'a, 'b> { }
impl<T> Trait<'_, '_> for T { }

async fn async_ret_impl_trait<'a, 'b>(a: &'a u8, b: &'b u8) -> impl Trait<'a, 'b> {
// { dg-error ".E0700." "" { target *-*-* } .-1 }
// { dg-error ".E0700." "" { target *-*-* } .-2 }
// { dg-error ".E0700." "" { target *-*-* } .-3 }
// { dg-error ".E0700." "" { target *-*-* } .-4 }
// { dg-error ".E0700." "" { target *-*-* } .-5 }
    (a, b)
}

fn main() {
    let _ = async_ret_impl_trait(&22, &44);
}

