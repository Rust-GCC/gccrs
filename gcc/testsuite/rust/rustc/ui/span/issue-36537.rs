fn main() {
    let p;
    {
        let a = 42;
        p = &a;
// { dg-error ".E0597." "" { target *-*-* } .-1 }

    }
    p.use_ref();

}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

