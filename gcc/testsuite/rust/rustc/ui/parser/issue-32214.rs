trait Trait<T> { type Item; }

pub fn test<W, I: Trait<Item=(), W> >() {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() { }

