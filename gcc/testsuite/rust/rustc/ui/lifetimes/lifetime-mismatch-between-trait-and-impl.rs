trait Foo {
    fn foo<'a>(x: &i32, y: &'a i32) -> &'a i32;
}

impl Foo for () {
    fn foo<'a>(x: &'a i32, y: &'a i32) -> &'a i32 {
// { dg-error "" "" { target *-*-* } .-1 }
        if x > y { x } else { y }
    }
}

fn main() {}

