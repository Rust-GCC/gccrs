#![feature(box_syntax)]
use std::any::Any;

fn main()
{
    fn h(x:i32) -> i32 {3*x}
    let mut vfnfer:Vec<Box<dyn Any>> = vec![];
    vfnfer.push(box h);
    println!("{:?}",(vfnfer[0] as dyn Fn)(3));
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
// { dg-error ".E0191." "" { target *-*-* } .-3 }
}

