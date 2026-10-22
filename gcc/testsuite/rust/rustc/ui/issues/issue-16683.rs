trait T<'a> {
    fn a(&'a self) -> &'a bool;
    fn b(&self) {
        self.a(); // { dg-error ".E0495." "" { target *-*-* } }
    }
}

fn main() {}

