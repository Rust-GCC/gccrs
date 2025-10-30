// Note: the borrowck analysis is currently flow-insensitive.
// Therefore, some of these errors are marked as spurious and could be
// corrected by a simple change to the analysis.  The others are
// either genuine or would require more advanced changes.  The latter
// cases are noted.

#![feature(box_syntax)]

fn borrow(_v: &isize) {}
fn borrow_mut(_v: &mut isize) {}
fn cond() -> bool { panic!() }
fn for_func<F>(_f: F) where F: FnOnce() -> bool { panic!() }
fn produce<T>() -> T { panic!(); }

fn inc(v: &mut Box<isize>) {
    *v = box (**v + 1);
}

fn pre_freeze_cond() {
    // In this instance, the freeze is conditional and starts before
    // the mut borrow.

    let u = box 0;
    let mut v: Box<_> = box 3;
    let mut _w = &u;
    if cond() {
        _w = &v;
    }
    borrow_mut(&mut *v); // { dg-error ".E0502." "" { target *-*-* } }
    _w.use_ref();
}

fn pre_freeze_else() {
    // In this instance, the freeze and mut borrow are on separate sides
    // of the if.

    let u = box 0;
    let mut v: Box<_> = box 3;
    let mut _w = &u;
    if cond() {
        _w = &v;
    } else {
        borrow_mut(&mut *v);
    }
    _w.use_ref();
}

fn main() {}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

