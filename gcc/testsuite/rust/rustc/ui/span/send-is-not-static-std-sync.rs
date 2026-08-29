// basic tests to see that certain "obvious" errors are caught by
// these types no longer requiring `'static` (RFC 458)

#![allow(dead_code)]

use std::sync::{Mutex, RwLock, mpsc};

fn mutex() {
    let x = 1;
    let y = Box::new(1);
    let lock = Mutex::new(&x);
    *lock.lock().unwrap() = &*y;
    drop(y); // { dg-error ".E0505." "" { target *-*-* } }
    {
        let z = 2;
        *lock.lock().unwrap() = &z;
    }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
    lock.use_ref(); // (Mutex is #[may_dangle] so its dtor does not use `z` => needs explicit use)
}

fn rwlock() {
    let x = 1;
    let y = Box::new(1);
    let lock = RwLock::new(&x);
    *lock.write().unwrap() = &*y;
    drop(y); // { dg-error ".E0505." "" { target *-*-* } }
    {
        let z = 2;
        *lock.write().unwrap() = &z;
    }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
    lock.use_ref(); // (RwLock is #[may_dangle] so its dtor does not use `z` => needs explicit use)
}

fn channel() {
    let x = 1;
    let y = Box::new(1);
    let (tx, rx) = mpsc::channel();

    tx.send(&x).unwrap();
    tx.send(&*y);
    drop(y); // { dg-error ".E0505." "" { target *-*-* } }
    {
        let z = 2;
        tx.send(&z).unwrap();
    }
// { dg-error ".E0597." "" { target *-*-* } .-2 }
    // (channels lack #[may_dangle], thus their dtors are implicit uses of `z`)
}

fn main() {}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

