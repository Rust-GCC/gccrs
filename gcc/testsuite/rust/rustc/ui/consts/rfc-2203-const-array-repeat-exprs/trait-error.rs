#![feature(const_in_array_repeat_expressions)]

#[derive(Copy, Clone)]
struct Foo<T>(T);

fn main() {
    [Foo(String::new()); 4];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

