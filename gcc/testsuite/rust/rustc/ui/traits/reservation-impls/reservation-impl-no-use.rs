// check that reservation impls can't be used as normal impls in positive reasoning.

#![feature(rustc_attrs)]

trait MyTrait { fn foo(&self); }
#[rustc_reservation_impl = "foo"]
impl MyTrait for () { fn foo(&self) {} }

fn main() {
    <() as MyTrait>::foo(&());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

