// Regression test for issue 67856

#![feature(unboxed_closures)]
#![feature(type_alias_impl_trait)]
#![feature(fn_traits)]

trait MyTrait {}
impl MyTrait for () {}

impl<F> FnOnce<()> for &F {
// { dg-error ".E0210." "" { target *-*-* } .-1 }
// { dg-error ".E0210." "" { target *-*-* } .-2 }
    type Output = impl MyTrait;
    extern "rust-call" fn call_once(self, _: ()) -> Self::Output {}
}
fn main() {}

