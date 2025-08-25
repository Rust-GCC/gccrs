// Check that `&mut` objects cannot be borrowed twice, just like
// other `&mut` pointers.



trait Foo {
    fn f1(&mut self) -> &();
    fn f2(&mut self);
}

fn test(x: &mut dyn Foo) {
    let y = x.f1();
    x.f2(); // { dg-error ".E0499." "" { target *-*-* } }
    y.use_ref();
}

fn main() {}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

