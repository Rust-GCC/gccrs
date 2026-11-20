pub struct List<T> {
    head: T,
}

impl Drop for List<i32> { // { dg-error ".E0366." "" { target *-*-* } }
    fn drop(&mut self) {
        panic!()
    }
}

fn main() {
    List { head: 0 };
}

