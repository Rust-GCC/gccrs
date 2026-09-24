// check that accesses due to a closure capture give a special note

fn closure_imm_capture_conflict(mut x: i32) {
    let r = &mut x;
    || x; // { dg-error ".E0502." "" { target *-*-* } }
    r.use_mut();
}

fn closure_mut_capture_conflict(mut x: i32) {
    let r = &mut x;
    || x = 2; // { dg-error ".E0499." "" { target *-*-* } }
    r.use_mut();
}

fn closure_unique_capture_conflict(mut x: &mut i32) {
    let r = &mut x;
    || *x = 2; // { dg-error ".E0500." "" { target *-*-* } }
    r.use_mut();
}

fn closure_copy_capture_conflict(mut x: i32) {
    let r = &mut x;
    move || x; // { dg-error ".E0503." "" { target *-*-* } }
    r.use_ref();
}

fn closure_move_capture_conflict(mut x: String) {
    let r = &x;
    || x; // { dg-error ".E0505." "" { target *-*-* } }
    r.use_ref();
}

fn closure_imm_capture_moved(mut x: String) {
    let r = x;
    || x.len(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn closure_mut_capture_moved(mut x: String) {
    let r = x;
    || x = String::new(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn closure_unique_capture_moved(x: &mut String) {
    let r = x;
    || *x = String::new(); // { dg-error ".E0382." "" { target *-*-* } }
}

fn closure_move_capture_moved(x: &mut String) {
    let r = x;
    || x; // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

