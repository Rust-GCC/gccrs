use foo::bar; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

use bar::Baz as x; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { suggestion "" "" { target *-*-* } .-4 }

use food::baz; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { suggestion "" "" { target *-*-* } .-4 }

use food::{beens as Foo}; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
// { suggestion "" "" { target *-*-* } .-4 }

mod bar {
    pub struct Bar;
}

mod food {
    pub use self::zug::baz::{self as bag, Foobar as beans};

    mod zug {
        pub mod baz {
            pub struct Foobar;
        }
    }
}

mod m {
    enum MyEnum {
        MyVariant
    }

    use MyEnum::*; // { dg-error ".E0432." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }
}

mod items {
    enum Enum {
        Variant
    }

    use Enum::*; // { dg-error ".E0432." "" { target *-*-* } }
// { help "" "" { target *-*-* } .-2 }
// { suggestion "" "" { target *-*-* } .-3 }

    fn item() {}
}

fn main() {}

