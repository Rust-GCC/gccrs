mod foo {
    pub struct X<'a, 'b, 'c, T> {
        a: &'a str,
        b: &'b str,
        c: &'c str,
        t: T,
    }
}

fn bar<'a, 'b, 'c, T>(x: foo::X<'a, T, 'b, 'c>) {}
// { dg-error ".E0747." "" { target *-*-* } .-1 }

fn main() {}

