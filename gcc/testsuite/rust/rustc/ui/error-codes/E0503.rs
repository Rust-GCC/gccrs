fn main() {
    let mut value = 3;
    let _borrow = &mut value;
    let _sum = value + 1; // { dg-error ".E0503." "" { target *-*-* } }
    _borrow.use_mut();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

