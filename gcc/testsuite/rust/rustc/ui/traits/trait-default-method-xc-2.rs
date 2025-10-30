// run-pass
// aux-build:trait_default_method_xc_aux.rs
// aux-build:trait_default_method_xc_aux_2.rs



extern crate trait_default_method_xc_aux as aux;
extern crate trait_default_method_xc_aux_2 as aux2;
use aux::A;
use aux2::{a_struct, welp};


pub fn main () {

    let a = a_struct { x: 0 };
    let b = a_struct { x: 1 };

    assert_eq!(0.g(), 10);
    assert_eq!(a.g(), 10);
    assert_eq!(a.h(), 11);
    assert_eq!(b.g(), 10);
    assert_eq!(b.h(), 11);
    assert_eq!(A::lurr(&a, &b), 21);

    welp(&0);
}

