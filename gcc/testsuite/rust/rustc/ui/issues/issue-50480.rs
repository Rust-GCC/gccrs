#[derive(Clone, Copy)]
// { dg-error ".E0204." "" { target *-*-* } .-1 }
struct Foo(NotDefined, <i32 as Iterator>::Item, Vec<i32>, String);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }

fn main() {}

