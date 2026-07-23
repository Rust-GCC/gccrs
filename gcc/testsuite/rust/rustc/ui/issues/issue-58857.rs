struct Conj<A> {a : A}
trait Valid {}

impl<A: !Valid> Conj<A>{}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

