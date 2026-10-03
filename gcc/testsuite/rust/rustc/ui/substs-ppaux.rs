//
// revisions: verbose normal
//
//[verbose] compile-flags: -Z verbose

trait Foo<'b, 'c, S=u32> {
    fn bar<'a, T>() where T: 'a {}
    fn baz() {}
}

impl<'a,'b,T,S> Foo<'a, 'b, S> for T {}

fn main() {}

fn foo<'z>() where &'z (): Sized {
    let x: () = <i8 as Foo<'static, 'static,  u8>>::bar::<'static, char>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }


    let x: () = <i8 as Foo<'static, 'static,  u32>>::bar::<'static, char>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

    let x: () = <i8 as Foo<'static, 'static,  u8>>::baz;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

    let x: () = foo::<'static>;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }
// { dg-error "" "" { target *-*-* } .-4 }
// { dg-error "" "" { target *-*-* } .-2 }
// { dg-error "" "" { target *-*-* } .-3 }

    <str as Foo<u8>>::bar;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

