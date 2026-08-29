trait Foo {
    fn foo();
}

use Foo::foo; // { dg-error ".E0253." "" { target *-*-* } }

fn main() { foo(); }

