// run-pass

#![warn(indirect_structural_match)]

struct CustomEq;

impl Eq for CustomEq {}
impl PartialEq for CustomEq {
    fn eq(&self, _: &Self) -> bool {
        false
    }
}

#[derive(PartialEq, Eq)]
enum Foo {
    Bar,
    Baz,
    Qux(CustomEq),
}

const BAR_BAZ: Foo = if 42 == 42 {
    Foo::Bar
} else {
    Foo::Baz
};

fn main() {
    match Foo::Qux(CustomEq) {
        BAR_BAZ => panic!(),
        _ => {}
    }
}

