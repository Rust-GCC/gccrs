struct S<X, Y> {
    x: X,
    y: Y,
}

fn main() {
    let x: &&Box<i32>;
    let _y = &**x; // { dg-error ".E0381." "" { target *-*-* } }

    let x: &&S<i32, i32>;
    let _y = &**x; // { dg-error ".E0381." "" { target *-*-* } }

    let x: &&i32;
    let _y = &**x; // { dg-error ".E0381." "" { target *-*-* } }


    let mut a: S<i32, i32>;
    a.x = 0;            // { dg-error ".E0381." "" { target *-*-* } }
    let _b = &a.x;

    let mut a: S<&&i32, &&i32>;
    a.x = &&0;          // { dg-error ".E0381." "" { target *-*-* } }
    let _b = &**a.x;


    let mut a: S<i32, i32>;
    a.x = 0;            // { dg-error ".E0381." "" { target *-*-* } }
    let _b = &a.y;

    let mut a: S<&&i32, &&i32>;
    a.x = &&0;          // { dg-error ".E0381." "" { target *-*-* } }
    let _b = &**a.y;
}

