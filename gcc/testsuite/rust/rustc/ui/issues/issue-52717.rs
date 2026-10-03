enum A {
  A {
    foo: usize,
  }
}

fn main() {
  let x = A::A { foo: 3 };
  match x {
    A::A { fob } => { println!("{}", fob); }
// { dg-error ".E0026." "" { target *-*-* } .-1 }
  }
}

