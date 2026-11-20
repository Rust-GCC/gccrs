struct Foo;

impl<T: Default> Foo { // { dg-error ".E0207." "" { target *-*-* } }
    fn get(&self) -> T {
        <T as Default>::default()
    }
}

fn main() {
}

