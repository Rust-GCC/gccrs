// Tests that we consider `Box<U>: !Sugar` to be ambiguous, even
// though we see no impl of `Sugar` for `Box`. Therefore, an overlap
// error is reported for the following pair of impls (#23516).

pub trait Sugar { fn dummy(&self) { } }
pub trait Sweet { fn dummy(&self) { } }
impl<T:Sugar> Sweet for T { }
impl<U:Sugar> Sweet for Box<U> { }
// { dg-error ".E0119." "" { target *-*-* } .-1 }

fn main() { }

