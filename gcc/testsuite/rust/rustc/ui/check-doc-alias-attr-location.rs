#![crate_type="lib"]
#![feature(doc_alias)]

pub struct Bar;
pub trait Foo {
    type X;
    fn foo() -> Self::X;
}

#[doc(alias = "foo")] // { dg-error "" "" { target *-*-* } }
extern {}

#[doc(alias = "bar")] // { dg-error "" "" { target *-*-* } }
impl Bar {
    #[doc(alias = "const")]
    const A: u32 = 0;
}

#[doc(alias = "foobar")] // { dg-error "" "" { target *-*-* } }
impl Foo for Bar {
    #[doc(alias = "assoc")] // { dg-error "" "" { target *-*-* } }
    type X = i32;
    fn foo() -> Self::X { 0 }
}

