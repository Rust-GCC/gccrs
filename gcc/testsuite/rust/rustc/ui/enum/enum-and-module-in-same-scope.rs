enum Foo {
    X
}

mod Foo { // { dg-error ".E0428." "" { target *-*-* } }
    pub static X: isize = 42;
    fn f() { f() } // Check that this does not result in a resolution error
}

fn main() {}

