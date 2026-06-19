struct Bar<'a> {
    s: &'a String
}

impl<'a> Bar<'a> {
    fn f(&mut self) {
        self.s.push('x');
// { dg-error ".E0596." "" { target *-*-* } .-1 }
    }
}

fn main() {}

