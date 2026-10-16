fn main() {
    struct::foo();
// { dg-error "" "" { target *-*-* } .-1 }
}
fn bar() {
    mut::baz();
// { dg-error "" "" { target *-*-* } .-1 }
}

