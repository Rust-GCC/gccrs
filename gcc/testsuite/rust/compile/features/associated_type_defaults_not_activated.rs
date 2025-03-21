// check-pass

// This is another instance of the "normalizations don't work" issue with
// defaulted associated types.

// #![feature(associated_type_defaults)]

pub trait Emitter<'a> {
    type Ctxt: 'a;
    type CtxtBrw: 'a = &'a Self::Ctxt;

    fn get_cx(&'a self) -> Self::CtxtBrw;
}

fn main() {}
