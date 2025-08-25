// Tests the different rules for `fn` forms requiring the presence or lack of a body.

fn main() {
    fn f1(); // { dg-error "" "" { target *-*-* } }
    fn f2() {} // OK.

    trait X {
        fn f1(); // OK.
        fn f2() {} // OK.
    }

    struct Y;
    impl X for Y {
        fn f1(); // { dg-error "" "" { target *-*-* } }
        fn f2() {} // OK.
    }

    impl Y {
        fn f3(); // { dg-error "" "" { target *-*-* } }
        fn f4() {} // OK.
    }

    extern {
        fn f5(); // OK.
        fn f6() {} // { dg-error "" "" { target *-*-* } }
    }
}

