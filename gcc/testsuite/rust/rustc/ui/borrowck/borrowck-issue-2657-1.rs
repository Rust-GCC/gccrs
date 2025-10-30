#![feature(box_syntax)]



fn main() {
    let x: Option<Box<_>> = Some(box 1);
    match x {
      Some(ref _y) => {
        let _a = x; // { dg-error ".E0505." "" { target *-*-* } }
        _y.use_ref();
      }
      _ => {}
    }
}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

