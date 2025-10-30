// This test documents that `type Out = Box<dyn Bar<Assoc: Copy>>;`
// is allowed and will correctly reject an opaque `type Out` which
// does not satisfy the bound `<TheType as Bar>::Assoc: Copy`.
//
// FIXME(rust-lang/lang): I think this behavior is logical if we want to allow
// `dyn Trait<Assoc: Bound>` but we should decide if we want that. // Centril
//
// Additionally, as reported in https://github.com/rust-lang/rust/issues/63594,
// we check that the spans for the error message are sane here.

#![feature(associated_type_bounds)]

fn main() {}

trait Bar {
    type Assoc;
}

trait Thing {
    type Out;
    fn func() -> Self::Out;
}

struct AssocNoCopy;
impl Bar for AssocNoCopy {
    type Assoc = String;
}

impl Thing for AssocNoCopy {
    type Out = Box<dyn Bar<Assoc: Copy>>;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    fn func() -> Self::Out {
        Box::new(AssocNoCopy)
    }
}

