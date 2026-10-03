trait Foo {
    const NAME: &'static str;
}


impl<'a> Foo for &'a () {
    const NAME: &'a str = "unit";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

