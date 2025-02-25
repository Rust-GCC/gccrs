trait Foo {
    fn bar(self: *const Self); // { dg-error "arbitrary self types are experimental." }
}
