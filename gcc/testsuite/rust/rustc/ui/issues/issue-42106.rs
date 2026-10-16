fn do_something<T>(collection: &mut Vec<T>) {
    let _a = &collection;
    collection.swap(1, 2); // { dg-error ".E0502." "" { target *-*-* } }
    _a.use_ref();
}

fn main() { }

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

