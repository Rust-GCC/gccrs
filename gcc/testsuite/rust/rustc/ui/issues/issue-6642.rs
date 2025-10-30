struct A;
impl A {
    fn m(&self) {
        fn x() {
            self.m() // { dg-error ".E0434." "" { target *-*-* } }
        }
    }
}
fn main() {}

