trait TraitA {
    const VALUE: usize;
}

struct A;
impl TraitA for A {
  const VALUE: usize = 0;
}

trait TraitB {
    type MyA: TraitA;
    const VALUE: usize = Self::MyA::VALUE;
}

struct B;
impl TraitB for B { // { dg-error ".E0046." "" { target *-*-* } }
    type M   = A; // { dg-error ".E0437." "" { target *-*-* } }
}

fn main() {
    let _ = [0; B::VALUE];
}

