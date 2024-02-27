#[lang = "sized"]
pub trait Sized {}

mod encase {

// regular functions

pub fn foo0() -> str { // { dg-error {return [^\n]* dynamically} }
    // TODO: this should produce other
    //       errors at some point
    *""
}

pub fn foo1(_: str) {} // { dg-error {parameter [^\n]* dynamically} }

// trait functions

pub trait Foo {
    fn undef(self, _: str); // this is ok
    fn method(self) {} // { dg-error {parameter [^\n]* dynamically} }

    fn foo0() -> str { // { dg-error {return [^\n]* dynamically} }
        // TODO: this should produce other
        //       errors at some point
        *""
    }

    fn foo1(_: str) {} // { dg-error {parameter [^\n]* dynamically} }
}

// impl functions

struct S;

impl S {
    fn foo0(_: str) {} // { dg-error {parameter [^\n]* dynamically} }

    fn foo1() -> str { // { dg-error {return [^\n]* dynamically} }
        // TODO: this should produce other
        //       errors at some point
        *""
    }
}

} // mod encase

pub fn main() -> i32 {
    0
}
