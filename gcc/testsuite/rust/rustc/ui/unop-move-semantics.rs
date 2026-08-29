// Test that move restrictions are enforced on overloaded unary operations

use std::ops::Not;

fn move_then_borrow<T: Not<Output=T> + Clone>(x: T) {
    !x;

    x.clone();  // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_borrowed<T: Not<Output=T>>(x: T, mut y: T) {
    let m = &x;
    let n = &mut y;

    !x;  // { dg-error ".E0505." "" { target *-*-* } }

    !y;  // { dg-error ".E0505." "" { target *-*-* } }
    use_mut(n); use_imm(m);
}
fn illegal_dereference<T: Not<Output=T>>(mut x: T, y: T) {
    let m = &mut x;
    let n = &y;

    !*m;  // { dg-error ".E0507." "" { target *-*-* } }

    !*n;  // { dg-error ".E0507." "" { target *-*-* } }
    use_imm(n); use_mut(m);
}
fn main() {}

fn use_mut<T>(_: &mut T) { }
fn use_imm<T>(_: &T) { }

