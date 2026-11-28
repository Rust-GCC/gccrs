// Test slicing expressions doesn't defeat the borrow checker.

fn main() {
    let y;
    {
        let x: &[isize] = &vec![1, 2, 3, 4, 5];
// { dg-error ".E0716." "" { target *-*-* } .-1 }
        y = &x[1..];
    }
    y.use_ref();
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

