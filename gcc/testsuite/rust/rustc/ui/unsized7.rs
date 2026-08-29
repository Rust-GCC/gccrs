// Test sized-ness checking in substitution in impls.

trait T {}

// I would like these to fail eventually.
// impl - bounded
trait T1<Z: T> {
    fn dummy(&self) -> Z;
}

struct S3<Y: ?Sized>(Box<Y>);
impl<X: ?Sized + T> T1<X> for S3<X> {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() { }

