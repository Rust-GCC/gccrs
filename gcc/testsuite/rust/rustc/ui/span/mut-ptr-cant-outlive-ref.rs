use std::cell::RefCell;

fn main() {
    let m = RefCell::new(0);
    let p;
    {
        let b = m.borrow();
        p = &*b;
    }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
    p.use_ref();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

