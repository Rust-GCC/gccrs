mod foo {
    pub struct Bx(());
}

mod bar {
    use foo::Bx;

    fn foo() {
        Bx(());
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    }
}

mod baz {
    fn foo() {
        Bx(());
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

fn main() {}

