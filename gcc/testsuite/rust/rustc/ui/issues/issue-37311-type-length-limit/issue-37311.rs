// build-fail
// normalize-stderr-test: ".nll/" -> "/"

trait Mirror {
    type Image;
}

impl<T> Mirror for T { type Image = T; }

trait Foo {
    fn recurse(&self);
}

impl<T> Foo for T {
    #[allow(unconditional_recursion)]
    fn recurse(&self) {
        (self, self).recurse(); // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {
    ().recurse();
}

