use std::pin::Pin;
use std::rc::Rc;
use std::ops::Add;

struct Foo;

impl Add for Foo {
    type Output = ();
    fn add(self, _rhs: Self) -> () {}
}

impl Foo {
    fn use_self(self) {}
    fn use_box_self(self: Box<Self>) {}
    fn use_pin_box_self(self: Pin<Box<Self>>) {}
    fn use_rc_self(self: Rc<Self>) {}
    fn use_mut_self(&mut self) -> &mut Self { self }
}

struct Container(Vec<bool>);

impl Container {
    fn custom_into_iter(self) -> impl Iterator<Item = bool> {
        self.0.into_iter()
    }
}

fn move_out(val: Container) {
    val.0.into_iter().next();
    val.0; // { dg-error ".E0382." "" { target *-*-* } }

    let foo = Foo;
    foo.use_self();
    foo; // { dg-error ".E0382." "" { target *-*-* } }

    let second_foo = Foo;
    second_foo.use_self();
    second_foo; // { dg-error ".E0382." "" { target *-*-* } }

    let boxed_foo = Box::new(Foo);
    boxed_foo.use_box_self();
    boxed_foo; // { dg-error ".E0382." "" { target *-*-* } }

    let pin_box_foo = Box::pin(Foo);
    pin_box_foo.use_pin_box_self();
    pin_box_foo; // { dg-error ".E0382." "" { target *-*-* } }

    let mut mut_foo = Foo;
    let ret = mut_foo.use_mut_self();
    mut_foo; // { dg-error ".E0505." "" { target *-*-* } }
    ret;

    let rc_foo = Rc::new(Foo);
    rc_foo.use_rc_self();
    rc_foo; // { dg-error ".E0382." "" { target *-*-* } }

    let foo_add = Foo;
    foo_add + Foo;
    foo_add; // { dg-error ".E0382." "" { target *-*-* } }

    let implicit_into_iter = vec![true];
    for _val in implicit_into_iter {}
    implicit_into_iter; // { dg-error ".E0382." "" { target *-*-* } }

    let explicit_into_iter = vec![true];
    for _val in explicit_into_iter.into_iter() {}
    explicit_into_iter; // { dg-error ".E0382." "" { target *-*-* } }

    let container = Container(vec![]);
    for _val in container.custom_into_iter() {}
    container; // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {}

