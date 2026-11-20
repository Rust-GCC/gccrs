// revisions: full min
#![cfg_attr(full, allow(incomplete_features))]
#![cfg_attr(full, feature(const_generics))]
#![cfg_attr(min, feature(min_const_generics))]

fn test<const N: usize>() {
    struct Foo<const M: usize>;
    macro_rules! foo {
        ($x:expr) => {
            [u8; $x] // { dg-error "" "" { target *-*-* } }
        }
    }
    macro_rules! bar {
        ($x:expr) => {
            [u8; { $x }] // { dg-error "" "" { target *-*-* } }
        }
    }
    macro_rules! baz {
        ( $x:expr) => {
            Foo<$x> // { dg-error "" "" { target *-*-* } }
        }
    }
    macro_rules! biz {
        ($x:expr) => {
            Foo<{ $x }> // { dg-error "" "" { target *-*-* } }
        };
    }

    let _: foo!(N);
    let _: foo!({ N });
    let _: foo!({{ N }}); // { dg-error "" "" { target *-*-* } }
    let _: bar!(N);
    let _: bar!({ N }); // { dg-error "" "" { target *-*-* } }
    let _: baz!(N); // { dg-error "" "" { target *-*-* } }
    let _: baz!({ N });
    let _: baz!({{ N }}); // { dg-error "" "" { target *-*-* } }
    let _: biz!(N);
    let _: biz!({ N }); // { dg-error "" "" { target *-*-* } }
}

fn main() {
    test::<3>();
}

