#[inline = ""] 
// { dg-error "invalid syntax, .inline. attribute does not accept value with .=., use parentheses instead: ...inline.always... or ...inline.never... .E0535." "" { target *-*-* } .-1 }
fn main() {}
