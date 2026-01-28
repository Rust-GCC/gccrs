// { dg-options "-fsyntax-only" }
pub fn a() {
    let v = 10;
    let ref mut r = v; // { dg-error "cannot borrow immutable local variable as mutable" }
}

pub fn b() {
    let mut v2 = 10;
    let ref mut r2 = v2; // Should compile fine
}