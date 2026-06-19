struct Foo;

impl Foo {
    fn bar(self) {}
    fn baz(&self, x: f64) {}
}

trait FooT {
    fn bag(&self);
}

impl FooT for Foo {
    fn bag(&self) {}
}

fn main() {
    let f = Foo;
    f.bat(1.0); // { dg-error ".E0599." "" { target *-*-* } }

    let s = "foo".to_string();
    let _ = s.is_emtpy(); // { dg-error ".E0599." "" { target *-*-* } }

    // Generates a warning for `count_zeros()`. `count_ones()` is also a close
    // match, but the former is closer.
    let _ = 63u32.count_eos(); // { dg-error ".E0599." "" { target *-*-* } }

    // Does not generate a warning
    let _ = 63u32.count_o(); // { dg-error ".E0599." "" { target *-*-* } }

}

