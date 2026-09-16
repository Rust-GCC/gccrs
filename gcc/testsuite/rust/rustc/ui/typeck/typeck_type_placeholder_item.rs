// Needed for `type Y = impl Trait<_>` and `type B = _;`
#![feature(type_alias_impl_trait, associated_type_defaults)]
// This test checks that it is not possible to enable global type
// inference by using the `_` type placeholder.

fn test() -> _ { 5 }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn test2() -> (_, _) { (5, 5) }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

static TEST3: _ = "test";
// { dg-error ".E0121." "" { target *-*-* } .-1 }

static TEST4: _ = 145;
// { dg-error ".E0121." "" { target *-*-* } .-1 }

static TEST5: (_, _) = (1, 2);
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn test6(_: _) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn test6_b<T>(_: _, _: T) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn test6_c<T, K, L, A, B>(_: _, _: (T, K, L, A, B)) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn test7(x: _) { let _x: usize = x; }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn test8(_f: fn() -> _) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }

struct Test9;

impl Test9 {
    fn test9(&self) -> _ { () }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn test10(&self, _x : _) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

fn test11(x: &usize) -> &_ {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    &x
}

unsafe fn test12(x: *const usize) -> *const *const _ {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    &x
}

impl Clone for Test9 {
    fn clone(&self) -> _ { Test9 }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn clone_from(&mut self, other: _) { *self = Test9; }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

struct Test10 {
    a: _,
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    b: (_, _),
}

pub fn main() {
    static A = 42;
// { dg-error "" "" { target *-*-* } .-1 }
    static B: _ = 42;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    static C: Option<_> = Some(42);
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn fn_test() -> _ { 5 }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn fn_test2() -> (_, _) { (5, 5) }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    static FN_TEST3: _ = "test";
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    static FN_TEST4: _ = 145;
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    static FN_TEST5: (_, _) = (1, 2);
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn fn_test6(_: _) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn fn_test7(x: _) { let _x: usize = x; }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn fn_test8(_f: fn() -> _) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }

    struct FnTest9;

    impl FnTest9 {
        fn fn_test9(&self) -> _ { () }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

        fn fn_test10(&self, _x : _) { }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    }

    impl Clone for FnTest9 {
        fn clone(&self) -> _ { FnTest9 }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

        fn clone_from(&mut self, other: _) { *self = FnTest9; }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    }

    struct FnTest10 {
        a: _,
// { dg-error ".E0121." "" { target *-*-* } .-1 }
        b: (_, _),
    }

    fn fn_test11(_: _) -> (_, _) { panic!() }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }

    fn fn_test12(x: i32) -> (_, _) { (x, x) }
// { dg-error ".E0121." "" { target *-*-* } .-1 }

    fn fn_test13(x: _) -> (i32, _) { (x, x) }
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

trait T {
    fn method_test1(&self, x: _);
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    fn method_test2(&self, x: _) -> _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    fn method_test3(&self) -> _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    fn assoc_fn_test1(x: _);
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    fn assoc_fn_test2(x: _) -> _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    fn assoc_fn_test3() -> _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

struct BadStruct<_>(_);
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }
trait BadTrait<_> {}
// { dg-error "" "" { target *-*-* } .-1 }
impl BadTrait<_> for BadStruct<_> {}
// { dg-error ".E0121." "" { target *-*-* } .-1 }

fn impl_trait() -> impl BadTrait<_> {
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    unimplemented!()
}

struct BadStruct1<_, _>(_);
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }
// { dg-error ".E0121." "" { target *-*-* } .-3 }
// { dg-error ".E0121." "" { target *-*-* } .-4 }
struct BadStruct2<_, T>(_, T);
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }

type X = Box<_>;
// { dg-error ".E0121." "" { target *-*-* } .-1 }

struct Struct;
trait Trait<T> {}
impl Trait<usize> for Struct {}
type Y = impl Trait<_>;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
fn foo() -> Y {
    Struct
}

trait Qux {
    type A;
    type B = _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    const C: _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    const D: _ = 42;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    // type E: _; // FIXME: make the parser propagate the existence of `B`
    type F: std::ops::Fn(_);
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}
impl Qux for Struct {
    type A = _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    type B = _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
    const C: _;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
// { dg-error ".E0121." "" { target *-*-* } .-2 }
    const D: _ = 42;
// { dg-error ".E0121." "" { target *-*-* } .-1 }
}

