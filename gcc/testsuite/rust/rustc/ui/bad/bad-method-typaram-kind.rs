fn foo<T:'static>() {
    1.bar::<T>(); // { dg-error ".E0277." "" { target *-*-* } }
}

trait Bar {
    fn bar<T:Send>(&self);
}

impl Bar for usize {
    fn bar<T:Send>(&self) {
    }
}

fn main() {}

