trait Trait {
    fn bar<'a,'b:'a>(x: &'a str, y: &'b str);
// { dg-note "" "" { target *-*-* } .-1 }
}

struct Foo;

impl Trait for Foo {
    fn bar<'a,'b>(x: &'a str, y: &'b str) { // { dg-error ".E0195." "" { target *-*-* } }
// { dg-note ".E0195." "" { target *-*-* } .-1 }
    }
}

fn main() {
}

