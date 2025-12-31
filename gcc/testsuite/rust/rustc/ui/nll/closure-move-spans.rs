// check that moves due to a closure capture give a special note

fn move_after_move(x: String) {
    || x;
    let y = x; // { dg-error ".E0382." "" { target *-*-* } }
}

fn borrow_after_move(x: String) {
    || x;
    let y = &x; // { dg-error ".E0382." "" { target *-*-* } }
}

fn borrow_mut_after_move(mut x: String) {
    || x;
    let y = &mut x; // { dg-error ".E0382." "" { target *-*-* } }
}

fn fn_ref<F: Fn()>(f: F) -> F { f }
fn fn_mut<F: FnMut()>(f: F) -> F { f }

fn main() {}

