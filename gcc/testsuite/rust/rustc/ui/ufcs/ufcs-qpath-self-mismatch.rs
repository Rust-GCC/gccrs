use std::ops::Add;

fn main() {
    <i32 as Add<u32>>::add(1, 2);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    <i32 as Add<i32>>::add(1u32, 2);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    <i32 as Add<i32>>::add(1, 2u32);
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

