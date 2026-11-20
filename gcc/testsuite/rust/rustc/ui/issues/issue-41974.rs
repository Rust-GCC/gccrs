#[derive(Copy, Clone)]
struct Flags;

trait A {
}

impl<T> Drop for T where T: A { // { dg-error ".E0210." "" { target *-*-* } }
// { dg-error ".E0210." "" { target *-*-* } .-1 }
// { dg-error ".E0210." "" { target *-*-* } .-2 }
    fn drop(&mut self) {
    }
}

fn main() {}

