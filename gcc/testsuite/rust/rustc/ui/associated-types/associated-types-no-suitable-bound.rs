trait Get {
    type Value;
    fn get(&self) -> <Self as Get>::Value;
}

struct Struct {
    x: isize,
}

impl Struct {
    fn uhoh<T>(foo: <T as Get>::Value) {}
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
}

