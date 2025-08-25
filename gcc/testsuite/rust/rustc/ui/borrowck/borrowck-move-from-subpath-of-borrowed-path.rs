// verify that an error is raised when trying to move out of a
// borrowed path.



#![feature(box_syntax)]

fn main() {
    let a: Box<Box<_>> = box box 2;
    let b = &a;

    let z = *a; // { dg-error ".E0505." "" { target *-*-* } }
    b.use_ref();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

