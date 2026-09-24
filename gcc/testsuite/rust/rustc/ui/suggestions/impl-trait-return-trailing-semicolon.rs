trait Bar {}
impl Bar for u8 {}
fn foo() -> impl Bar {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

