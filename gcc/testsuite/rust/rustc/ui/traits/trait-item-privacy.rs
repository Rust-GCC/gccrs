#![feature(associated_type_defaults)]

struct S;

mod method {
    trait A {
        fn a(&self) { }
    }

    pub trait B {
        fn b(&self) { }
    }

    pub trait C: A + B {
        fn c(&self) { }
    }

    impl A for ::S {}
    impl B for ::S {}
    impl C for ::S {}
}

mod assoc_const {
    trait A {
        const A: u8 = 0;
    }

    pub trait B {
        const B: u8 = 0;
    }

    pub trait C: A + B {
        const C: u8 = 0;
    }

    impl A for ::S {}
    impl B for ::S {}
    impl C for ::S {}
}

mod assoc_ty {
    trait A {
        type A = u8;
    }

    pub trait B {
        type B = u8;
    }

    pub trait C: A + B {
        type C = u8;
    }

    impl A for ::S {}
    impl B for ::S {}
    impl C for ::S {}
}

fn check_method() {
    // A is private
    // B is pub, not in scope
    // C : A + B is pub, in scope
    use method::C;

    // Methods, method call
    // a, b, c are resolved as trait items, their traits need to be in scope
    S.a(); // { dg-error ".E0599." "" { target *-*-* } }
    S.b(); // { dg-error ".E0599." "" { target *-*-* } }
    S.c(); // OK
    // a, b, c are resolved as inherent items, their traits don't need to be in scope
    let c = &S as &dyn C;
    c.a(); // { dg-error ".E0624." "" { target *-*-* } }
    c.b(); // OK
    c.c(); // OK

    // Methods, UFCS
    // a, b, c are resolved as trait items, their traits need to be in scope
    S::a(&S);
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    S::b(&S);
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    S::c(&S); // OK
    // a, b, c are resolved as inherent items, their traits don't need to be in scope
    C::a(&S); // { dg-error ".E0624." "" { target *-*-* } }
    C::b(&S); // OK
    C::c(&S); // OK
}

fn check_assoc_const() {
    // A is private
    // B is pub, not in scope
    // C : A + B is pub, in scope
    use assoc_const::C;

    // Associated constants
    // A, B, C are resolved as trait items, their traits need to be in scope
    S::A; // { dg-error ".E0599." "" { target *-*-* } }
    S::B; // { dg-error ".E0599." "" { target *-*-* } }
    S::C; // OK
    // A, B, C are resolved as inherent items, their traits don't need to be in scope
    C::A; // { dg-error ".E0038." "" { target *-*-* } }
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    C::B; // ERROR the trait `assoc_const::C` cannot be made into an object
    C::C; // OK
}

fn check_assoc_ty<T: assoc_ty::C>() {
    // A is private
    // B is pub, not in scope
    // C : A + B is pub, in scope
    use assoc_ty::C;

    // Associated types
    // A, B, C are resolved as trait items, their traits need to be in scope, not implemented yet
    let _: S::A; // { dg-error ".E0223." "" { target *-*-* } }
    let _: S::B; // { dg-error ".E0223." "" { target *-*-* } }
    let _: S::C; // { dg-error ".E0223." "" { target *-*-* } }
    // A, B, C are resolved as inherent items, their traits don't need to be in scope
    let _: T::A; // { dg-error "" "" { target *-*-* } }
    let _: T::B; // OK
    let _: T::C; // OK

    // Associated types, bindings
    let _: dyn assoc_ty::B<
        B = u8, // OK
    >;
    let _: dyn C<
        A = u8, // { dg-error "" "" { target *-*-* } }
        B = u8, // OK
        C = u8, // OK
    >;
}

fn main() {}

