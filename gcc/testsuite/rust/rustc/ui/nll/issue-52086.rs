use std::rc::Rc;
use std::sync::Arc;

struct Bar { field: Vec<i32> }

fn main() {
    let x = Rc::new(Bar { field: vec![] });
    drop(x.field);
// { dg-error ".E0507." "" { target *-*-* } .-1 }

    let y = Arc::new(Bar { field: vec![] });
    drop(y.field);
// { dg-error ".E0507." "" { target *-*-* } .-1 }
}

