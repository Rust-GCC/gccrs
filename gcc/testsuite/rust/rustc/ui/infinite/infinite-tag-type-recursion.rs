enum MList { Cons(isize, MList), Nil }
// { dg-error ".E0391." "" { target *-*-* } .-1 }
// { dg-error ".E0391." "" { target *-*-* } .-2 }

fn main() { let a = MList::Cons(10, MList::Cons(11, MList::Nil)); }

