#![feature(min_const_generics)]
#![feature(never_type)]

struct Foo<const N: [u8; 0]>;
// { dg-error "" "" { target *-*-* } .-1 }

struct Bar<const N: ()>;
// { dg-error "" "" { target *-*-* } .-1 }
#[derive(PartialEq, Eq)]
struct No;

struct Fez<const N: No>;
// { dg-error "" "" { target *-*-* } .-1 }

struct Faz<const N: &'static u8>;
// { dg-error "" "" { target *-*-* } .-1 }

struct Fiz<const N: !>;
// { dg-error "" "" { target *-*-* } .-1 }

enum Goo<const N: ()> { A, B }
// { dg-error "" "" { target *-*-* } .-1 }

union Boo<const N: ()> { a: () }
// { dg-error "" "" { target *-*-* } .-1 }


fn main() {}

