use std::rc::Rc;

union U<T: Copy> {
    a: T
}

fn main() {
    let u = U { a: Rc::new(0u32) };
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    let u = U::<Rc<u32>> { a: Default::default() };
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

