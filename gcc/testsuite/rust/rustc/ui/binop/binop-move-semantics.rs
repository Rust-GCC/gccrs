// Test that move restrictions are enforced on overloaded binary operations

use std::ops::Add;

fn double_move<T: Add<Output=()>>(x: T) {
    x
    +
    x;  // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_then_borrow<T: Add<Output=()> + Clone>(x: T) {
    x
    +
    x.clone();  // { dg-error ".E0382." "" { target *-*-* } }
}

fn move_borrowed<T: Add<Output=()>>(x: T, mut y: T) {
    let m = &x;
    let n = &mut y;

    x  // { dg-error ".E0505." "" { target *-*-* } }
    +
    y;  // { dg-error ".E0505." "" { target *-*-* } }
    use_mut(n); use_imm(m);
}
fn illegal_dereference<T: Add<Output=()>>(mut x: T, y: T) {
    let m = &mut x;
    let n = &y;

    *m  // { dg-error ".E0507." "" { target *-*-* } }
    +
    *n;  // { dg-error ".E0507." "" { target *-*-* } }
    use_imm(n); use_mut(m);
}
struct Foo;

impl<'a, 'b> Add<&'b Foo> for &'a mut Foo {
    type Output = ();

    fn add(self, _: &Foo) {}
}

impl<'a, 'b> Add<&'b mut Foo> for &'a Foo {
    type Output = ();

    fn add(self, _: &mut Foo) {}
}

fn mut_plus_immut() {
    let mut f = Foo;

    &mut f
    +
    &f;  // { dg-error ".E0502." "" { target *-*-* } }
}

fn immut_plus_mut() {
    let mut f = Foo;

    &f
    +
    &mut f;  // { dg-error ".E0502." "" { target *-*-* } }
}

fn main() {}

fn use_mut<T>(_: &mut T) { }
fn use_imm<T>(_: &T) { }

