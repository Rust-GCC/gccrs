mod a {
    pub fn foo() {}
}

mod b {
    pub fn foo() {}
}

mod c {
    pub use a::foo;
}

mod d {
    use a::foo;
    use a::foo; // { dg-error ".E0252." "" { target *-*-* } }
}

mod e {
    pub use a::*;
    pub use c::*; // ok
}

mod f {
    pub use a::*;
    pub use b::*;
}

mod g {
    pub use a::*;
    pub use f::*;
}

fn main() {
    e::foo();
    f::foo(); // { dg-error ".E0659." "" { target *-*-* } }
    g::foo();
}

mod ambiguous_module_errors {
    pub mod m1 { pub use super::m1 as foo; pub fn bar() {} }
    pub mod m2 { pub use super::m2 as foo; }

    use self::m1::*;
    use self::m2::*;

    use self::foo::bar; // { dg-error ".E0659." "" { target *-*-* } }

    fn f() {
        foo::bar(); // { dg-error ".E0659." "" { target *-*-* } }
    }
}

