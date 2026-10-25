#![feature(no_core)]
#![no_core]

const K: i32 = 1;
static S: i32 = 2;

fn main() {
    const Y: i32 = K + S;
    const Z: i32 = {
        const fn inner() -> i32 {
            let z = 3;
            z
        }
        inner()
    };
    let _ = Y + Z;
}
