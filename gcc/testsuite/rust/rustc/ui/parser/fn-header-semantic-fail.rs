// Ensures that all `fn` forms can have all the function qualifiers syntactically.

// edition:2018

#![feature(const_extern_fn)]
#![feature(const_fn)]

fn main() {
    async fn ff1() {} // OK.
    unsafe fn ff2() {} // OK.
    const fn ff3() {} // OK.
    extern "C" fn ff4() {} // OK.
    const async unsafe extern "C" fn ff5() {} // OK.
// { dg-error "" "" { target *-*-* } .-1 }

    trait X {
        async fn ft1(); // { dg-error ".E0706." "" { target *-*-* } }
        unsafe fn ft2(); // OK.
        const fn ft3(); // { dg-error ".E0379." "" { target *-*-* } }
        extern "C" fn ft4(); // OK.
        const async unsafe extern "C" fn ft5();
// { dg-error ".E0706." "" { target *-*-* } .-1 }
// { dg-error ".E0379." "" { target *-*-* } .-2 }
// { dg-error ".E0706." "" { target *-*-* } .-3 }
    }

    struct Y;
    impl X for Y {
        async fn ft1() {} // { dg-error ".E0053." "" { target *-*-* } }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
        unsafe fn ft2() {} // OK.
        const fn ft3() {} // { dg-error ".E0379." "" { target *-*-* } }
        extern "C" fn ft4() {}
        const async unsafe extern "C" fn ft5() {}
// { dg-error ".E0053." "" { target *-*-* } .-1 }
// { dg-error ".E0053." "" { target *-*-* } .-2 }
// { dg-error ".E0053." "" { target *-*-* } .-3 }
// { dg-error ".E0053." "" { target *-*-* } .-4 }
    }

    impl Y {
        async fn fi1() {} // OK.
        unsafe fn fi2() {} // OK.
        const fn fi3() {} // OK.
        extern "C" fn fi4() {} // OK.
        const async unsafe extern "C" fn fi5() {}
// { dg-error "" "" { target *-*-* } .-1 }
    }

    extern {
        async fn fe1(); // { dg-error "" "" { target *-*-* } }
        unsafe fn fe2(); // { dg-error "" "" { target *-*-* } }
        const fn fe3(); // { dg-error "" "" { target *-*-* } }
        extern "C" fn fe4(); // { dg-error "" "" { target *-*-* } }
        const async unsafe extern "C" fn fe5(); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    }
}

