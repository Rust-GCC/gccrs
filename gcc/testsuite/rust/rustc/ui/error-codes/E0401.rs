trait Baz<T> {}

fn foo<T>(x: T) {
    fn bfnr<U, V: Baz<U>, W: Fn()>(y: T) { // { dg-error ".E0401." "" { target *-*-* } }
    }
    fn baz<U,
           V: Baz<U>,
           W: Fn()>
           (y: T) { // { dg-error ".E0401." "" { target *-*-* } }
    }
    bfnr(x); // { dg-error ".E0282." "" { target *-*-* } }
}


struct A<T> {
    inner: T,
}

impl<T> Iterator for A<T> {
    type Item = u8;
    fn next(&mut self) -> Option<u8> {
        fn helper(sel: &Self) -> u8 { // { dg-error ".E0401." "" { target *-*-* } }
            unimplemented!();
        }
        Some(helper(self))
    }
}

fn main() {
}

