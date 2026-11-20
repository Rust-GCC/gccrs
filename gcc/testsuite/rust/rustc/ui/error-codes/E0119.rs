trait MyTrait {
    fn get(&self) -> usize;
}

impl<T> MyTrait for T {
    fn get(&self) -> usize { 0 }
}

struct Foo {
    value: usize
}

impl MyTrait for Foo { // { dg-error ".E0119." "" { target *-*-* } }
    fn get(&self) -> usize { self.value }
}

fn main() {
}

