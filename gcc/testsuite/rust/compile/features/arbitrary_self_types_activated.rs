#![feature(arbitrary_self_types)]

trait Foo {
    fn bar(self: *const Self);
}
