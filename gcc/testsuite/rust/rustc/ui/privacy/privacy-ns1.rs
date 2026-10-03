// Check we do the correct privacy checks when we import a name and there is an
// item with that name in both the value and type namespaces.

#![allow(dead_code)]
#![allow(unused_imports)]


// public type, private value
pub mod foo1 {
    pub trait Bar {
    }
    pub struct Baz;

    fn Bar() { }
}

fn test_glob1() {
    use foo1::*;

    Bar();  // { dg-error ".E0423." "" { target *-*-* } }
}

// private type, public value
pub mod foo2 {
    trait Bar {
    }
    pub struct Baz;

    pub fn Bar() { }
}

fn test_glob2() {
    use foo2::*;

    let _x: Box<Bar>;  // { dg-error ".E0107." "" { target *-*-* } }
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

// neither public
pub mod foo3 {
    trait Bar {
    }
    pub struct Baz;

    fn Bar() { }
}

fn test_glob3() {
    use foo3::*;

    Bar();  // { dg-error ".E0425." "" { target *-*-* } }
    let _x: Box<Bar>;  // { dg-error ".E0412." "" { target *-*-* } }
}

fn main() {
}

