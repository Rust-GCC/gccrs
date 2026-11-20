use std::collections::BTreeSet;

#[derive(Hash)]
pub enum ElemDerived { // { dg-error ".E0072." "" { target *-*-* } }
    A(ElemDerived)
}

pub enum Elem {
    Derived(ElemDerived)
}

pub struct Set(BTreeSet<Elem>);

impl Set {
    pub fn into_iter(self) -> impl Iterator<Item = Elem> {
        self.0.into_iter()
    }
}

fn main() {}

