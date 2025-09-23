trait Trait {
    fn baz(&self, _: Self) {}
    fn bat(&self) -> Self {}
}

fn bar(x: &dyn Trait) {} // { dg-error ".E0038." "" { target *-*-* } }

trait Other: Sized {}

fn foo(x: &dyn Other) {} // { dg-error ".E0038." "" { target *-*-* } }

fn main() {}

