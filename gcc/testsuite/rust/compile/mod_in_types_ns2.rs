mod foo {
    mod bar {}

    fn baz() -> bar {} // { dg-error "expected type, found module" }
}
