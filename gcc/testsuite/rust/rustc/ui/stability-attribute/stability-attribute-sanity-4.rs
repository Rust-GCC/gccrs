// Various checks that stability attributes are used correctly, per RFC 507

#![feature(staged_api)]

#![stable(feature = "rust1", since = "1.0.0")]

mod bogus_attribute_types_2 {
    #[unstable] // { dg-error "" "" { target *-*-* } }
    fn f1() { }

    #[unstable = "b"] // { dg-error "" "" { target *-*-* } }
    fn f2() { }

    #[stable] // { dg-error "" "" { target *-*-* } }
    fn f3() { }

    #[stable = "a"] // { dg-error "" "" { target *-*-* } }
    fn f4() { }

    #[stable(feature = "a", since = "b")]
    #[rustc_deprecated] // { dg-error "" "" { target *-*-* } }
    fn f5() { }

    #[stable(feature = "a", since = "b")]
    #[rustc_deprecated = "a"] // { dg-error "" "" { target *-*-* } }
    fn f6() { }
}

fn main() { }

