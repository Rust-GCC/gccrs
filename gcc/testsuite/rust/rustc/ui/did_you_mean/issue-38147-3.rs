struct Qux<'a> {
    s: &'a String
}

impl<'a> Qux<'a> {
    fn f(&self) {
        self.s.push('x');
// { dg-error ".E0596." "" { target *-*-* } .-1 }
    }
}

fn main() {}

