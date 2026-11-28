#![feature(const_generics)]
// { dg-warning "" "" { target *-*-* } .-1 }

fn function_with_str<'a, const STRING: &'a str>() {} // { dg-error ".E0771." "" { target *-*-* } }

fn main() {
    function_with_str::<"Hello, world!">()
}

