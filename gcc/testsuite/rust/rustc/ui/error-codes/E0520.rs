#![feature(specialization)]
// { dg-warning "" "" { target *-*-* } .-1 }

trait SpaceLlama {
    fn fly(&self);
}

impl<T> SpaceLlama for T {
    default fn fly(&self) {}
}

impl<T: Clone> SpaceLlama for T {
    fn fly(&self) {}
}

impl SpaceLlama for i32 {
    default fn fly(&self) {}
// { dg-error ".E0520." "" { target *-*-* } .-1 }
}

fn main() {
}

