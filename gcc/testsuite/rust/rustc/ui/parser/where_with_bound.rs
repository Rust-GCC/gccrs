fn foo<T>() where <T>::Item: ToString, T: Iterator { }
// { dg-error ".E0412." "" { target *-*-* } .-1 }
// { dg-error ".E0412." "" { target *-*-* } .-2 }

fn main() {}

