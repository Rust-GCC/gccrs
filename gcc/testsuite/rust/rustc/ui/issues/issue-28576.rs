pub trait Foo<RHS=Self> {
    type Assoc;
}

pub trait Bar: Foo<Assoc=()> {
    fn new(&self, b: &
           dyn Bar // { dg-error ".E0038." "" { target *-*-* } }
              <Assoc=()>
    );
}

fn main() {}

