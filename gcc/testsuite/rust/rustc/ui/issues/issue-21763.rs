// Regression test for HashMap only impl'ing Send/Sync if its contents do

use std::collections::HashMap;
use std::rc::Rc;

fn foo<T: Send>() {}

fn main() {
    foo::<HashMap<Rc<()>, Rc<()>>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

