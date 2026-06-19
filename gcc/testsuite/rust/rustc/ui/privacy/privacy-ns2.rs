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

fn test_single1() {
    use foo1::Bar;

    Bar(); // { dg-error ".E0423." "" { target *-*-* } }
}

fn test_list1() {
    use foo1::{Bar,Baz};

    Bar(); // { dg-error ".E0423." "" { target *-*-* } }
}

// private type, public value
pub mod foo2 {
    trait Bar {
    }
    pub struct Baz;

    pub fn Bar() { }
}

fn test_single2() {
    use foo2::Bar;

    let _x : Box<Bar>; // { dg-error ".E0107." "" { target *-*-* } }
// { dg-error ".E0107." "" { target *-*-* } .-1 }
    let _x : Bar(); // { dg-error ".E0573." "" { target *-*-* } }
}

fn test_list2() {
    use foo2::{Bar,Baz};

    let _x: Box<Bar>; // { dg-error ".E0107." "" { target *-*-* } }
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

// neither public
pub mod foo3 {
    trait Bar {
    }
    pub struct Baz;

    fn Bar() { }
}

fn test_unused3() {
    use foo3::Bar;  // { dg-error ".E0603." "" { target *-*-* } }
}

fn test_single3() {
    use foo3::Bar;  // { dg-error ".E0603." "" { target *-*-* } }

    Bar();
    let _x: Box<Bar>;
}

fn test_list3() {
    use foo3::{Bar,Baz};  // { dg-error ".E0603." "" { target *-*-* } }

    Bar();
    let _x: Box<Bar>;
}

fn main() {
}

