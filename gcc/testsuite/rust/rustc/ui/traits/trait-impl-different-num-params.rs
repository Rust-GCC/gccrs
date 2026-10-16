trait Foo {
    fn bar(&self, x: usize) -> Self;
}
impl Foo for isize {
    fn bar(&self) -> isize {
// { dg-error ".E0050." "" { target *-*-* } .-1 }
        *self
    }
}

fn main() {
}

