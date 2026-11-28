fn foo<T>() where for<'a> T: 'a {}

fn bar<'a>() {
    foo::<&'a i32>();
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

fn main() {
    bar();
}

