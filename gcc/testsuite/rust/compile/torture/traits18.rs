trait Foo<'a> {}

trait Bar {
    type Item: for<'a> Foo<'a>;
}
