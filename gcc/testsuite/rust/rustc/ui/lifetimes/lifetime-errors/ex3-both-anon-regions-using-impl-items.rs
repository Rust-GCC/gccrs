trait Foo {
    fn foo<'a>(x: &mut Vec<&u8>, y: &u8);
}
impl Foo for () {
    fn foo(x: &mut Vec<&u8>, y: &u8) {
        x.push(y); // { dg-error ".E0623." "" { target *-*-* } }
    }
}
fn main() {}

