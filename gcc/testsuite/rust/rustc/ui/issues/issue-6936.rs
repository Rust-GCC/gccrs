struct T;

mod t1 {
    type Foo = ::T;
    mod Foo {} // { dg-error ".E0428." "" { target *-*-* } }
}

mod t2 {
    type Foo = ::T;
    struct Foo; // { dg-error ".E0428." "" { target *-*-* } }
}

mod t3 {
    type Foo = ::T;
    enum Foo {} // { dg-error ".E0428." "" { target *-*-* } }
}

mod t4 {
    type Foo = ::T;
    fn Foo() {} // ok
}

mod t5 {
    type Bar<T> = T;
    mod Bar {} // { dg-error ".E0428." "" { target *-*-* } }
}

mod t6 {
    type Foo = ::T;
    impl Foo {} // ok
}


fn main() {}

