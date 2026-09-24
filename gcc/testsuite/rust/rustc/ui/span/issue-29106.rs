use std::rc::Rc;
use std::sync::Arc;

struct Foo<'a>(&'a String);

impl<'a> Drop for Foo<'a> {
    fn drop(&mut self) {
        println!("{:?}", self.0);
    }
}

fn main() {
    {
        let (y, x);
        x = "alive".to_string();
        y = Arc::new(Foo(&x));
    }
// { dg-error ".E0597." "" { target *-*-* } .-2 }

    {
        let (y, x);
        x = "alive".to_string();
        y = Rc::new(Foo(&x));
    }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
}

