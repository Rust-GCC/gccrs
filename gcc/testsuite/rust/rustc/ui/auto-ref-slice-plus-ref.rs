fn main() {

    // Testing that method lookup does not automatically borrow
    // vectors to slices then automatically create a self reference.

    let mut a = vec![0];
    a.test_mut(); // { dg-error ".E0599." "" { target *-*-* } }
    a.test(); // { dg-error ".E0599." "" { target *-*-* } }

    ([1]).test(); // { dg-error ".E0599." "" { target *-*-* } }
    (&[1]).test(); // { dg-error ".E0599." "" { target *-*-* } }
}

trait MyIter {
    fn test_mut(&mut self);
    fn test(&self);
}

impl<'a> MyIter for &'a [isize] {
    fn test_mut(&mut self) { }
    fn test(&self) { }
}

impl<'a> MyIter for &'a str {
    fn test_mut(&mut self) { }
    fn test(&self) { }
}

