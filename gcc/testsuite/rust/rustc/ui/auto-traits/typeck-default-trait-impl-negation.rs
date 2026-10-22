#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

auto trait MyTrait {}

unsafe auto trait MyUnsafeTrait {}

struct ThisImplsTrait;

impl !MyUnsafeTrait for ThisImplsTrait {}


struct ThisImplsUnsafeTrait;

impl !MyTrait for ThisImplsUnsafeTrait {}

fn is_my_trait<T: MyTrait>() {}
fn is_my_unsafe_trait<T: MyUnsafeTrait>() {}

fn main() {
    is_my_trait::<ThisImplsTrait>();
    is_my_trait::<ThisImplsUnsafeTrait>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    is_my_unsafe_trait::<ThisImplsTrait>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    is_my_unsafe_trait::<ThisImplsUnsafeTrait>();
}

