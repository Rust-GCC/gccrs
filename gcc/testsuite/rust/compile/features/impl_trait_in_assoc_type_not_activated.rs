// { dg-additional-options "-frust-compile-until=lowering" }

fn main() {}

trait Bar {
    type Assoc;
}

trait Thing {
    type Out;
    fn func() -> Self::Out;
}

struct AssocIsCopy;
impl Bar for AssocIsCopy {
    type Assoc = u8;
}

impl Thing for AssocIsCopy {
    type Out = impl Bar<Assoc: Copy>; // { dg-error "impl trait in assoc type is experimental" }

    fn func() -> Self::Out {
        AssocIsCopy
    }
}
