trait A {
    fn a(&self) {
        || self.b()
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    }
}
fn main() {}

