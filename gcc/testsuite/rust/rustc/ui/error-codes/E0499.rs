fn main() {
    let mut i = 0;
    let mut x = &mut i;
    let mut a = &mut i; // { dg-error ".E0499." "" { target *-*-* } }
    a.use_mut();
    x.use_mut();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

