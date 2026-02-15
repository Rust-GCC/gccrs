struct A;
struct B;

static S: &'static B = &A;
// { dg-error ".E0015." "" { target *-*-* } .-1 }

use std::ops::Deref;

impl Deref for A {
    type Target = B;
    fn deref(&self)->&B { static B_: B = B; &B_ }
}

fn main(){}

