// Test that we elaborate `Type: 'region` constraints and infer various important things.

trait Master<'a, T: ?Sized, U> {
    fn foo() where T: 'a;
}

// `U::Item: 'a` does not imply that `U: 'a`
impl<'a, U: Iterator> Master<'a, U::Item, U> for () {
    fn foo() where U: 'a { } // { dg-error ".E0276." "" { target *-*-* } }
}

fn main() {
    println!("Hello, world!");
}

