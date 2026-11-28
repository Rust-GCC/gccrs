// Tests (negatively) the ability for the Self type in default methods
// to use capabilities granted by builtin kinds as supertraits.

use std::sync::mpsc::{channel, Sender};

trait Foo : Sized+Sync+'static {
    fn foo(self, mut chan: Sender<Self>) { }
}

impl <T: Sync> Foo for T { }
// { dg-error ".E0310." "" { target *-*-* } .-1 }

fn main() {
    let (tx, rx) = channel();
    1193182.foo(tx);
    assert_eq!(rx.recv(), 1193182);
}

