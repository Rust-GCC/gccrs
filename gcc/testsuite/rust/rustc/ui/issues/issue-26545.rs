mod foo {
    pub struct B(pub ());
}

mod baz {
    fn foo() {
        B(());
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {}

