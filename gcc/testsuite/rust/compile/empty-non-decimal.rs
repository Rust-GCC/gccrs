#![feature(no_core)]
#![no_core]

fn main() {
    let _a = 0x; // { dg-error "no valid digits found for number" }
    let _b = 0b; // { dg-error "no valid digits found for number" }
    let _c = 0o; // { dg-error "no valid digits found for number" }
    let _d = 0x_; // { dg-error "no valid digits found for number" }
    let _e = 0x_u32; // { dg-error "no valid digits found for number" }
}
