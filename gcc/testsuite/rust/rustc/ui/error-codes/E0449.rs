struct Bar;

trait Foo {
    fn foo();
}

pub impl Bar {} // { dg-error ".E0449." "" { target *-*-* } }

pub impl Foo for Bar { // { dg-error ".E0449." "" { target *-*-* } }
    pub fn foo() {} // { dg-error ".E0449." "" { target *-*-* } }
}

fn main() {
}

