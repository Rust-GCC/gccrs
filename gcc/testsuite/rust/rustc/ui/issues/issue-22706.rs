fn is_copy<T: ::std::marker<i32>::Copy>() {}
// { dg-error ".E0109." "" { target *-*-* } .-1 }
fn main() {}

