// aux-build:no_method_suggested_traits.rs
extern crate no_method_suggested_traits;

struct Foo;
enum Bar { X }

mod foo {
    pub trait Bar {
        fn method(&self) {}

        fn method2(&self) {}
    }

    impl Bar for u32 {}

    impl Bar for char {}
}

fn main() {
    // test the values themselves, and autoderef.


    1u32.method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }
    std::rc::Rc::new(&mut Box::new(&1u32)).method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
// { dg-error ".E0599." "" { target *-*-* } .-2 }

    'a'.method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&'a')).method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    1i32.method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&1i32)).method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    Foo.method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&Foo)).method();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    1u64.method2();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&1u64)).method2();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    no_method_suggested_traits::Foo.method2();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&no_method_suggested_traits::Foo)).method2();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    no_method_suggested_traits::Bar::X.method2();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&no_method_suggested_traits::Bar::X)).method2();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    Foo.method3();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&Foo)).method3();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    Bar::X.method3();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    std::rc::Rc::new(&mut Box::new(&Bar::X)).method3();
// { dg-error ".E0599." "" { target *-*-* } .-1 }

    // should have no help:
    1_usize.method3(); // { dg-error ".E0599." "" { target *-*-* } }
    std::rc::Rc::new(&mut Box::new(&1_usize)).method3(); // { dg-error ".E0599." "" { target *-*-* } }
    no_method_suggested_traits::Foo.method3();  // { dg-error ".E0599." "" { target *-*-* } }
    std::rc::Rc::new(&mut Box::new(&no_method_suggested_traits::Foo)).method3();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    no_method_suggested_traits::Bar::X.method3();  // { dg-error ".E0599." "" { target *-*-* } }
    std::rc::Rc::new(&mut Box::new(&no_method_suggested_traits::Bar::X)).method3();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
}

