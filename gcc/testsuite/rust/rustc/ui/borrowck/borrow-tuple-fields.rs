#![feature(box_syntax)]



struct Foo(Box<isize>, isize);

struct Bar(isize, isize);

fn main() {
    let x: (Box<_>, _) = (box 1, 2);
    let r = &x.0;
    let y = x; // { dg-error ".E0505." "" { target *-*-* } }

    r.use_ref();

    let mut x = (1, 2);
    let a = &x.0;
    let b = &mut x.0; // { dg-error ".E0502." "" { target *-*-* } }
    a.use_ref();

    let mut x = (1, 2);
    let a = &mut x.0;
    let b = &mut x.0; // { dg-error ".E0499." "" { target *-*-* } }
    a.use_ref();

    let x = Foo(box 1, 2);
    let r = &x.0;
    let y = x; // { dg-error ".E0505." "" { target *-*-* } }
    r.use_ref();

    let mut x = Bar(1, 2);
    let a = &x.0;
    let b = &mut x.0; // { dg-error ".E0502." "" { target *-*-* } }
    a.use_ref();

    let mut x = Bar(1, 2);
    let a = &mut x.0;
    let b = &mut x.0; // { dg-error ".E0499." "" { target *-*-* } }
    a.use_mut();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

