trait Foo {
    fn orange(&self);
    fn orange(&self); // { dg-error ".E0428." "" { target *-*-* } }
}

fn main() {}

