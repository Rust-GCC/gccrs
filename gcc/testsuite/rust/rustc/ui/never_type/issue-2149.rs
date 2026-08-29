trait VecMonad<A> {
    fn bind<B, F>(&self, f: F) where F: FnMut(A) -> Vec<B>;
}

impl<A> VecMonad<A> for Vec<A> {
    fn bind<B, F>(&self, mut f: F) where F: FnMut(A) -> Vec<B> {
        let mut r = panic!();
        for elt in self { r = r + f(*elt); }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
   }
}
fn main() {
    ["hi"].bind(|x| [x] );
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

