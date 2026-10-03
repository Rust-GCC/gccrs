// run-rustfix
trait TraitB {
    type Item;
}

trait TraitA<A> {
    type Type;
    fn bar<T>(_: T) -> Self;
    fn baz<T>(_: T) -> Self where T: TraitB, <T as TraitB>::Item: Copy;
}

struct S;
struct Type;

impl TraitA<()> for S { // { dg-error ".E0046." "" { target *-*-* } }
}

fn main() {}

