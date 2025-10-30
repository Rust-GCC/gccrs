// run-pass
// ignore-emscripten no threads support

#![feature(box_syntax)]

use std::thread;

pub fn main() { test05(); }

fn test05_start<F:FnOnce(isize)>(f: F) {
    f(22);
}

fn test05() {
    let three: Box<_> = box 3;
    let fn_to_send = move|n:isize| {
        println!("{}", *three + n); // will copy x into the closure
        assert_eq!(*three, 3);
    };
    thread::spawn(move|| {
        test05_start(fn_to_send);
    }).join().ok().unwrap();
}

