macro_rules! foo {
    ($e:expr) => { $e.foo() }
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

fn main() {
    let a = 1i32;
    foo!(a);

    foo!(1i32.foo());
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

