trait Groom {
    fn shave(other: usize);
}

pub struct Cat {
  whiskers: isize,
}

pub enum MaybeDog {
    Dog,
    NoDog
}

impl MaybeDog {
  fn bark() {
    // If this provides a suggestion, it's a bug as MaybeDog doesn't impl Groom
    shave();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
  }
}

impl Clone for Cat {
  fn clone(&self) -> Self {
    clone();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    loop {}
  }
}
impl Default for Cat {
  fn default() -> Self {
    default();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    loop {}
  }
}

impl Groom for Cat {
  fn shave(other: usize) {
    whiskers -= other;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    shave(4);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    purr();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
  }
}

impl Cat {
    fn static_method() {}

    fn purr_louder() {
        static_method();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        purr();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        purr();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
        purr();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    }
}

impl Cat {
  fn meow() {
    if self.whiskers > 3 {
// { dg-error ".E0424." "" { target *-*-* } .-1 }
        println!("MEOW");
    }
  }

  fn purr(&self) {
    grow_older();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    shave();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
  }

  fn burn_whiskers(&mut self) {
    whiskers = 0;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
  }

  pub fn grow_older(other:usize) {
    whiskers = 4;
// { dg-error ".E0425." "" { target *-*-* } .-1 }
    purr_louder();
// { dg-error ".E0425." "" { target *-*-* } .-1 }
  }
}

fn main() {
    self += 1;
// { dg-error ".E0424." "" { target *-*-* } .-1 }
}

