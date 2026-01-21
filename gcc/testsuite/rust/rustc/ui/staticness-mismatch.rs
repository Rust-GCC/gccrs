trait Foo {
    fn dummy(&self) { }
    fn bar();
}

impl Foo for isize {
    fn bar(&self) {}
// { dg-error ".E0185." "" { target *-*-* } .-1 }
}

fn main() {}

