// { dg-additional-options "-w" }
#![feature(no_core, lang_items)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

const fn loop_continue() -> i32 {
    let mut i = 0;
    let mut n = 0;
    loop {
        i += 1;
        if i > 5 {
            break;
        }
        if i == 2 {
            continue;
        }
        n += 1;
    }
    n
}

const fn labeled_break() -> i32 {
    let mut i = 0;
    'outer: loop {
        loop {
            i += 1;
            if i == 3 {
                break 'outer;
            }
        }
    }
    i
}

const fn labeled_continue() -> i32 {
    let mut i = 0;
    let mut n = 0;
    'outer: while i < 5 {
        i += 1;
        let mut j = 0;
        while j < 5 {
            j += 1;
            if j == 2 {
                continue 'outer;
            }
            n += 1;
        }
    }
    n
}

const A: i32 = loop_continue();
const B: i32 = labeled_break();
const C: i32 = labeled_continue();

fn main() -> i32 {
    if A != 4 {
        return 1;
    }
    if B != 3 {
        return 2;
    }
    if C != 5 {
        return 3;
    }
    0
}
