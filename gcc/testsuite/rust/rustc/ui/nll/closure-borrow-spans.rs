// check that existing borrows due to a closure capture give a special note

fn move_while_borrowed(x: String) {
    let f = || x.len();
    let y = x; // { dg-error ".E0505." "" { target *-*-* } }
    f.use_ref();
}

fn borrow_mut_while_borrowed(mut x: i32) {
    let f = || x;
    let y = &mut x; // { dg-error ".E0502." "" { target *-*-* } }
    f.use_ref();
}

fn drop_while_borrowed() {
    let f;
    {
        let x = 1;
        f = || x; // { dg-error ".E0597." "" { target *-*-* } }
    }
    f.use_ref();
}

fn assign_while_borrowed(mut x: i32) {
    let f = || x;
    x = 1; // { dg-error ".E0506." "" { target *-*-* } }
    f.use_ref();
}

fn copy_while_borrowed_mut(mut x: i32) {
    let f = || x = 0;
    let y = x; // { dg-error ".E0503." "" { target *-*-* } }
    f.use_ref();
}

fn borrow_while_borrowed_mut(mut x: i32) {
    let f = || x = 0;
    let y = &x; // { dg-error ".E0502." "" { target *-*-* } }
    f.use_ref();
}

fn borrow_mut_while_borrowed_mut(mut x: i32) {
    let f = || x = 0;
    let y = &mut x; // { dg-error ".E0499." "" { target *-*-* } }
    f.use_ref();
}

fn drop_while_borrowed_mut() {
    let f;
    {
        let mut x = 1;
        f = || x = 0; // { dg-error ".E0597." "" { target *-*-* } }
    }
    f.use_ref();
}

fn assign_while_borrowed_mut(mut x: i32) {
    let f = || x = 0;
    x = 1; // { dg-error ".E0506." "" { target *-*-* } }
    f.use_ref();
}

fn copy_while_borrowed_unique(x: &mut i32) {
    let f = || *x = 0;
    let y = x; // { dg-error ".E0505." "" { target *-*-* } }
    f.use_ref();
}

fn borrow_while_borrowed_unique(x: &mut i32) {
    let f = || *x = 0;
    let y = &x; // { dg-error ".E0501." "" { target *-*-* } }
    f.use_ref();
}

fn borrow_mut_while_borrowed_unique(mut x: &mut i32) {
    let f = || *x = 0;
    let y = &mut x; // { dg-error ".E0501." "" { target *-*-* } }
    f.use_ref();
}

fn drop_while_borrowed_unique() {
    let mut z = 1;
    let f;
    {
        let x = &mut z;
        f = || *x = 0; // { dg-error ".E0597." "" { target *-*-* } }
    }
    f.use_ref();
}

fn assign_while_borrowed_unique(x: &mut i32) {
    let f = || *x = 0;
    *x = 1; // { dg-error ".E0506." "" { target *-*-* } }
    f.use_ref();
}

fn main() {}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

