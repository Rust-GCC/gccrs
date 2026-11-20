// aux-build:two_macros.rs

mod foo {
    extern crate two_macros;
    pub use self::two_macros::m as panic;
}

mod m1 {
    use foo::panic; // ok
    fn f() { panic!(); }
}

mod m2 {
    use foo::*;
    fn f() { panic!(); } // { dg-error ".E0659." "" { target *-*-* } }
}

mod m3 {
    ::two_macros::m!(use foo::panic;);
    fn f() { panic!(); } // { dg-error ".E0659." "" { target *-*-* } }
}

mod m4 {
    macro_rules! panic { () => {} } // ok
    panic!();
}

mod m5 {
    macro_rules! m { () => {
        macro_rules! panic { () => {} }
    } }
    m!();
    panic!(); // { dg-error ".E0659." "" { target *-*-* } }
}

#[macro_use(n)]
extern crate two_macros;
mod bar {
    pub use two_macros::m as n;
}

mod m6 {
    use bar::n; // ok
    n!();
}

mod m7 {
    use bar::*;
    n!(); // { dg-error ".E0659." "" { target *-*-* } }
}

fn main() {}

