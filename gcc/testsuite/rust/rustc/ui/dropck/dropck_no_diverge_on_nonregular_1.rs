// Issue 22443: Reject code using non-regular types that would
// otherwise cause dropck to loop infinitely.

use std::marker::PhantomData;

struct Digit<T> {
    elem: T
}

struct Node<T:'static> { m: PhantomData<&'static T> }


enum FingerTree<T:'static> {
    Single(T),
    // Bug report said Digit after Box would stack overflow (versus
    // Digit before Box; see dropck_no_diverge_on_nonregular_2).
    Deep(
        Box<FingerTree<Node<T>>>,
        Digit<T>,
        )
}

fn main() {
    let ft = // { dg-error ".E0320." "" { target *-*-* } }
        FingerTree::Single(1);
// { dg-error ".E0320." "" { target *-*-* } .-1 }
}

