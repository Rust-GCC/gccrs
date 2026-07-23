// check that liveness due to a closure capture gives a special note

fn use_as_borrow_capture(mut x: i32) {
    let y = &x;
    x = 0; // { dg-error ".E0506." "" { target *-*-* } }
    || *y;
}

fn use_as_borrow_mut_capture(mut x: i32) {
    let y = &mut x;
    x = 0; // { dg-error ".E0506." "" { target *-*-* } }
    || *y = 1;
}

fn use_as_move_capture(mut x: i32) {
    let y = &x;
    x = 0; // { dg-error ".E0506." "" { target *-*-* } }
    move || *y;
}

fn main() {}

