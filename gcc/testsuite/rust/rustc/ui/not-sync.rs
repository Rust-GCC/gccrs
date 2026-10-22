use std::cell::{Cell, RefCell};
use std::rc::{Rc, Weak};
use std::sync::mpsc::{Receiver, Sender};

fn test<T: Sync>() {}

fn main() {
    test::<Cell<i32>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    test::<RefCell<i32>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    test::<Rc<i32>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    test::<Weak<i32>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    test::<Receiver<i32>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    test::<Sender<i32>>();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

