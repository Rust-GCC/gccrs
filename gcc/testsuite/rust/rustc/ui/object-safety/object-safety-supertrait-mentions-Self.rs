// Check that we correctly prevent users from making trait objects
// form traits that make use of `Self` in an argument or return position.

trait Bar<T> {
    fn bar(&self, x: &T);
}

trait Baz : Bar<Self> {
}

fn make_bar<T:Bar<u32>>(t: &T) -> &dyn Bar<u32> {
    t
}

fn make_baz<T:Baz>(t: &T) -> &dyn Baz {
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    t
}

fn main() {
}

