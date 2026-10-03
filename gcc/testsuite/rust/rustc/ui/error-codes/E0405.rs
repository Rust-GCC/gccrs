struct Foo;

impl SomeTrait for Foo {} // { dg-error ".E0405." "" { target *-*-* } }

fn main() {
}

