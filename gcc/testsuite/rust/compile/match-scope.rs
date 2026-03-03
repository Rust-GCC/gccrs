#![no_core]

pub fn main() -> i32 {
    match 12 {
        x => {}
    }
    x // { dg-error "cannot find value 'x'" }
}
