pub struct Foo<const N: usize>;

pub fn foo<const N: usize>() -> Foo<{ N + 1 }> { // { dg-error "cannot evaluate constant expression with generic parameters" }
    Foo
}
// { dg-excess-errors "Noisy error cascade" }
