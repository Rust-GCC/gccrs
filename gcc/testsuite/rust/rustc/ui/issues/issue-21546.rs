// Also works as a test for #14564

#[allow(non_snake_case)]
mod Foo { }

#[allow(dead_code)]
struct Foo;
// { dg-error ".E0428." "" { target *-*-* } .-1 }

#[allow(non_snake_case)]
mod Bar { }

#[allow(dead_code)]
struct Bar(i32);
// { dg-error ".E0428." "" { target *-*-* } .-1 }


#[allow(dead_code)]
struct Baz(i32);

#[allow(non_snake_case)]
mod Baz { }
// { dg-error ".E0428." "" { target *-*-* } .-1 }


#[allow(dead_code)]
struct Qux { x: bool }

#[allow(non_snake_case)]
mod Qux { }
// { dg-error ".E0428." "" { target *-*-* } .-1 }


#[allow(dead_code)]
struct Quux;

#[allow(non_snake_case)]
mod Quux { }
// { dg-error ".E0428." "" { target *-*-* } .-1 }


#[allow(dead_code)]
enum Corge { A, B }

#[allow(non_snake_case)]
mod Corge { }
// { dg-error ".E0428." "" { target *-*-* } .-1 }

fn main() { }

