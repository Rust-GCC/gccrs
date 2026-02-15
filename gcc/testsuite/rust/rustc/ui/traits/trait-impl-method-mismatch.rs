trait Mumbo {
    fn jumbo(&self, x: &usize) -> usize;
}

impl Mumbo for usize {
    // Cannot have a larger effect than the trait:
    unsafe fn jumbo(&self, x: &usize) { *self + *x; }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
// { dg-error ".E0053." "" { target *-*-* } .-2 }
// { dg-error ".E0053." "" { target *-*-* } .-3 }
}

fn main() {}

