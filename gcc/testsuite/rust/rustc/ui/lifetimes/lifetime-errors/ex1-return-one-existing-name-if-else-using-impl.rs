trait Foo {

    fn foo<'a>(x: &i32, y: &'a i32) -> &'a i32;

}

impl Foo for () {

    fn foo<'a>(x: &i32, y: &'a i32) -> &'a i32 {

        if x > y { x } else { y } // { dg-error ".E0623." "" { target *-*-* } }

    }

}

fn main() {}

