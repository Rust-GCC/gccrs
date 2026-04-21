// { dg-output "222\r*\n333\r*\n" }
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

fn main() -> i32 {
    let a = 'block_a: {
        if false {
            break 'block_a 111;
        }
        222
    };
    dump_number(a);
    let b = 'block_b: {
        if true {
            break 'block_b 333;
        }
        444
    };
    dump_number(b);
    // ISSUE : Rust-GCC/gccrs#4538
    // let c = 'block_c: {
    //     break 'block_c 555;
    // };
    // dump_number(c);
    0
}
