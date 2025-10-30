use std::mem;

struct A { x: i32, y: f64 }

#[cfg(not(works))]
unsafe fn access(n:*mut A) -> (i32, f64) {
    let x : i32 = n.x; // { dg-error ".E0609." "" { target *-*-* } }
    let y : f64 = n.y; // { dg-error ".E0609." "" { target *-*-* } }
    (x, y)
}

#[cfg(works)]
unsafe fn access(n:*mut A) -> (i32, f64) {
    let x : i32 = (*n).x;
    let y : f64 = (*n).y;
    (x, y)
}

fn main() {
    let a :  A = A { x: 3, y: 3.14 };
    let p : &A = &a;
    let (x,y) = unsafe {
        let n : *mut A = mem::transmute(p);
        access(n)
    };
    println!("x: {}, y: {}", x, y);
}

