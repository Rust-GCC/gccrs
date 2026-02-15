// revisions: full min

#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(min, feature(min_const_generics))]

struct Foo<const N: [u8; { // { dg-error "" "" { target *-*-* } }
    struct Foo<const N: usize>;

    impl<const N: usize> Foo<N> {
        fn value() -> usize {
            N
        }
    }

    Foo::<17>::value()
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}]>;

fn main() {}

