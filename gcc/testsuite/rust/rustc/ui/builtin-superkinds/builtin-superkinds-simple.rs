// Basic test for traits inheriting from the builtin kinds, checking
// the type contents of the implementing type (that's not a typaram).

trait Foo : Send { }

impl Foo for std::rc::Rc<i8> { }
// { dg-error ".E0277." "" { target *-*-* } .-1 }

fn main() { }

