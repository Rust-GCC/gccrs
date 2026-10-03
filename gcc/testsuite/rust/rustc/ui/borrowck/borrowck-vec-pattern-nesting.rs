#![feature(box_patterns)]
#![feature(box_syntax)]

fn a() {
    let mut vec = [box 1, box 2, box 3];
    match vec {
        [box ref _a, _, _] => {
// { dg-note "" "" { target *-*-* } .-1 }
            vec[0] = box 4; // { dg-error ".E0506." "" { target *-*-* } }
// { dg-note ".E0506." "" { target *-*-* } .-1 }
            _a.use_ref();
// { dg-note "" "" { target *-*-* } .-1 }
        }
    }
}

fn b() {
    let mut vec = vec![box 1, box 2, box 3];
    let vec: &mut [Box<isize>] = &mut vec;
    match vec {
        &mut [ref _b @ ..] => {
// { dg-error "" "" { target *-*-* } .-1 }
            vec[0] = box 4; // { dg-error ".E0506." "" { target *-*-* } }
// { dg-note ".E0506." "" { target *-*-* } .-1 }
            _b.use_ref();
// { dg-note "" "" { target *-*-* } .-1 }
        }
    }
}

fn c() {
    let mut vec = vec![box 1, box 2, box 3];
    let vec: &mut [Box<isize>] = &mut vec;
    match vec {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
// { dg-note ".E0508." "" { target *-*-* } .-2 }
        &mut [_a,
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { help "" "" { target *-*-* } .-3 }
            ..
        ] => {
        }
        _ => {}
    }
    let a = vec[0]; // { dg-error ".E0508." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
}

fn d() {
    let mut vec = vec![box 1, box 2, box 3];
    let vec: &mut [Box<isize>] = &mut vec;
    match vec {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
// { dg-note ".E0508." "" { target *-*-* } .-2 }
        &mut [
// { help "" "" { target *-*-* } .-1 }
         _b] => {}
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
        _ => {}
    }
    let a = vec[0]; // { dg-error ".E0508." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
}

fn e() {
    let mut vec = vec![box 1, box 2, box 3];
    let vec: &mut [Box<isize>] = &mut vec;
    match vec {
// { dg-error ".E0508." "" { target *-*-* } .-1 }
// { dg-note ".E0508." "" { target *-*-* } .-2 }
// { dg-note ".E0508." "" { target *-*-* } .-3 }
        &mut [_a, _b, _c] => {}
// { dg-note "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
        _ => {}
    }
    let a = vec[0]; // { dg-error ".E0508." "" { target *-*-* } }
// { dg-note "" "" { target *-*-* } .-2 }
// { dg-note "" "" { target *-*-* } .-3 }
// { help "" "" { target *-*-* } .-4 }
}

fn main() {}

trait Fake { fn use_mut(&mut self) { } fn use_ref(&self) { }  }
impl<T> Fake for T { }

