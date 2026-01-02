pub struct Foo<const N: usize>;

// { dg-error "cannot evaluate constant expression with generic parameters" }
pub fn foo<const N: usize>() -> Foo<{ N + 1 }> {
    Foo
}
