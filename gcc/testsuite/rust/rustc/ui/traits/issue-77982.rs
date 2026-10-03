use std::collections::HashMap;

fn what() {
    let descr = String::new();
    let mut opts = HashMap::<String, ()>::new();
    let opt = String::new();

    opts.get(opt.as_ref()); // { dg-error ".E0283." "" { target *-*-* } }
}

fn main() {
    let ips: Vec<_> = (0..100_000).map(|_| u32::from(0u32.into())).collect();
// { dg-error ".E0283." "" { target *-*-* } .-1 }
}

trait Foo<'a, T: ?Sized> {
    fn foo(&self) -> Box<T> {
        todo!()
    }
}

trait Bar<'a, T: ?Sized> {
    fn bar(&self) -> Box<T> {
        todo!()
    }
}

impl Foo<'static, u32> for () {}
impl<'a> Foo<'a, i16> for () {}

impl<'a> Bar<'static, u32> for &'a () {}
impl<'a> Bar<'a, i16> for &'a () {}

fn foo() {
    let _ = ().foo(); // { dg-error ".E0283." "" { target *-*-* } }
}

fn bar() {
    let _ = (&()).bar(); // { dg-error ".E0283." "" { target *-*-* } }
}

