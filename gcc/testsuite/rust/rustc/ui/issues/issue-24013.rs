fn main() {
    use std::mem::{transmute, swap};
    let a = 1;
    let b = 2;
    unsafe {swap::<&mut _>(transmute(&a), transmute(&b))};
// { dg-error ".E0282." "" { target *-*-* } .-1 }
}

