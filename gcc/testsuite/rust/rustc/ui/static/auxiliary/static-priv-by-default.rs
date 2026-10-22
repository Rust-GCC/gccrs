// aux-build:static_priv_by_default.rs

extern crate static_priv_by_default;

mod child {
    pub mod childs_child {
        static private: isize = 0;
        pub static public: isize = 0;
    }
}

fn foo(_: isize) {}

fn full_ref() {
    foo(static_priv_by_default::private); // { dg-error "" "" { target *-*-* } }
    foo(static_priv_by_default::public);
    foo(child::childs_child::private); // { dg-error "" "" { target *-*-* } }
    foo(child::childs_child::public);
}

fn medium_ref() {
    use child::childs_child;
    foo(childs_child::private); // { dg-error "" "" { target *-*-* } }
    foo(childs_child::public);
}

fn main() {}

