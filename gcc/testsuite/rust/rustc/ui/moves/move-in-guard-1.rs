#![feature(box_syntax)]

pub fn main() {
    let x: Box<_> = box 1;

    let v = (1, 2);

    match v {
        (1, _) if take(x) => (),
        (_, 2) if take(x) => (), // { dg-error ".E0382." "" { target *-*-* } }
        _ => (),
    }
}

fn take<T>(_: T) -> bool { false }

