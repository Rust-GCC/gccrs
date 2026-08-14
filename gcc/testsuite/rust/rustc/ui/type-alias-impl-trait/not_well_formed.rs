#![feature(type_alias_impl_trait)]

fn main() {
}

trait TraitWithAssoc {
    type Assoc;
}

type Foo<V> = impl Trait<V::Assoc>; // { dg-error ".E0220." "" { target *-*-* } }

trait Trait<U> {}

impl<W> Trait<W> for () {}

fn foo_desugared<T: TraitWithAssoc>(_: T) -> Foo<T> {
    ()
}

