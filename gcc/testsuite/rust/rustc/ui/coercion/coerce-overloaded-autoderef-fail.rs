fn borrow_mut<T>(x: &mut T) -> &mut T { x }
fn borrow<T>(x: &T) -> &T { x }

fn borrow_mut2<T>(_: &mut T, _: &mut T) {}
fn borrow2<T>(_: &mut T, _: &T) {}

fn double_mut_borrow<T>(x: &mut Box<T>) {
    let y = borrow_mut(x);
    let z = borrow_mut(x);
// { dg-error ".E0499." "" { target *-*-* } .-1 }
    drop((y, z));
}

fn double_imm_borrow(x: &mut Box<i32>) {
    let y = borrow(x);
    let z = borrow(x);
    **x += 1;
// { dg-error ".E0506." "" { target *-*-* } .-1 }
    drop((y, z));
}

fn double_mut_borrow2<T>(x: &mut Box<T>) {
    borrow_mut2(x, x);
// { dg-error ".E0499." "" { target *-*-* } .-1 }
}

fn double_borrow2<T>(x: &mut Box<T>) {
    borrow2(x, x);
// { dg-error ".E0502." "" { target *-*-* } .-1 }
}

pub fn main() {}

