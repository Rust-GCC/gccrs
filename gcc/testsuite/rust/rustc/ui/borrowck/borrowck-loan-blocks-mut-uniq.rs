#![feature(box_syntax)]

fn borrow<F>(v: &isize, f: F) where F: FnOnce(&isize) {
    f(v);
}

fn box_imm() {
    let mut v: Box<_> = box 3;
    borrow(&*v,
           |w| { // { dg-error ".E0502." "" { target *-*-* } }
            v = box 4;
            assert_eq!(*v, 3);
            assert_eq!(*w, 4);
        })
}

fn main() {
}

