#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

auto trait MyTrait {}

struct MyS;

struct MyS2;

impl !MyTrait for MyS2 {}

fn is_mytrait<T: MyTrait>() {}

fn main() {
    is_mytrait::<MyS>();

    is_mytrait::<(MyS2, MyS)>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

