use Trait::foo; // { dg-error ".E0253." "" { target *-*-* } }
use Trait::Assoc; // { dg-error ".E0253." "" { target *-*-* } }
use Trait::C; // { dg-error ".E0253." "" { target *-*-* } }

use Foo::new; // { dg-error ".E0432." "" { target *-*-* } }

use Foo::C2; // { dg-error ".E0432." "" { target *-*-* } }

pub trait Trait {
    fn foo();
    type Assoc;
    const C: u32;
}

struct Foo;

impl Foo {
    fn new() {}
    const C2: u32 = 0;
}

fn main() {}

