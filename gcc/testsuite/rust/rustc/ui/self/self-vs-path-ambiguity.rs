// Check that `self::foo` is parsed as a general pattern and not a self argument.

struct S;

impl S {
    fn f(self::S: S) {}
    fn g(&self::S: &S) {}
    fn h(&mut self::S: &mut S) {}
    fn i(&'a self::S: &S) {} // { dg-error "" "" { target *-*-* } }
}

fn main() {}

