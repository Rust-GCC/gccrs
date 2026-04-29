// { dg-output "45\r*\n55\r*\n" }
#![feature(no_core)]
#![no_core]

extern "C" {
    fn printf(s: *const i8, ...);
}

fn dump_number(num: i32) {
    unsafe {
        let a = "%i\n\0";
        let c = a as *const str as *const i8;
        printf(c, num);
    }
}

const fn play(b: i32) -> i32 {
    let mut res = 0;
    let mut i = 0;
    while i < b {
        res += i;
        i += 1;
    }
    res
}

fn main() -> i32 {
    const A: i32 = play(10);
    dump_number(A);

    let b: i32 = play(11);
    dump_number(b);

    0
}
