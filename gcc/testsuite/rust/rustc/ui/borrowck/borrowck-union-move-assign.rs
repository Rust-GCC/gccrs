#![feature(untagged_unions)]

// Non-copy
struct A;
struct B;

union U {
    a: A,
    b: B,
}

fn main() {
    unsafe {
        {
            let mut u = U { a: A };
            let a = u.a;
            let a = u.a; // { dg-error ".E0382." "" { target *-*-* } }
        }
        {
            let mut u = U { a: A };
            let a = u.a;
            u.a = A;
            let a = u.a; // OK
        }
        {
            let mut u = U { a: A };
            let a = u.a;
            u.b = B;
            let a = u.a; // OK
        }
    }
}

