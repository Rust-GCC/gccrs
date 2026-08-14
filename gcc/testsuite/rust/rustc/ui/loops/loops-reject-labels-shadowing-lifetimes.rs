// Issue #21633: reject duplicate loop labels in function bodies.
// This is testing interaction between lifetime-params and labels.

// check-pass

#![allow(dead_code, unused_variables)]

fn foo() {
    fn foo<'a>() {
        'a: loop { break 'a; }
// { dg-warning "" "" { target *-*-* } .-1 }
    }

    struct Struct<'b, 'c> { _f: &'b i8, _g: &'c i8 }
    enum Enum<'d, 'e> { A(&'d i8), B(&'e i8) }

    impl<'d, 'e> Struct<'d, 'e> {
        fn meth_okay() {
            'a: loop { break 'a; }
            'b: loop { break 'b; }
            'c: loop { break 'c; }
        }
    }

    impl <'d, 'e> Enum<'d, 'e> {
        fn meth_okay() {
            'a: loop { break 'a; }
            'b: loop { break 'b; }
            'c: loop { break 'c; }
        }
    }

    impl<'bad, 'c> Struct<'bad, 'c> {
        fn meth_bad(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }

    impl<'b, 'bad> Struct<'b, 'bad> {
        fn meth_bad2(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }

    impl<'b, 'c> Struct<'b, 'c> {
        fn meth_bad3<'bad>(x: &'bad i8) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }

        fn meth_bad4<'a,'bad>(x: &'a i8, y: &'bad i8) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }

    impl <'bad, 'e> Enum<'bad, 'e> {
        fn meth_bad(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }
    impl <'d, 'bad> Enum<'d, 'bad> {
        fn meth_bad2(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }
    impl <'d, 'e> Enum<'d, 'e> {
        fn meth_bad3<'bad>(x: &'bad i8) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }

        fn meth_bad4<'a,'bad>(x: &'bad i8) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }

    trait HasDefaultMethod1<'bad> {
        fn meth_okay() {
            'c: loop { break 'c; }
        }
        fn meth_bad(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }
    trait HasDefaultMethod2<'a,'bad> {
        fn meth_bad(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }
    trait HasDefaultMethod3<'a,'b> {
        fn meth_bad<'bad>(&self) {
            'bad: loop { break 'bad; }
// { dg-warning "" "" { target *-*-* } .-1 }
        }
    }
}


pub fn main() {
    foo();
}

