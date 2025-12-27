//@compile-flags: -frust-incomplete-and-experimental-compiler-do-not-use
// ICE regression test: PR #4302

pub struct Foo<const N: usize>;

pub fn foo<const N: usize>() -> Foo<{ N + 1 }> {
    Foo
}
