const C: i32 = 1i8; // { dg-error ".E0308." "" { target *-*-* } }
const D: i8 = C; // { dg-error ".E0308." "" { target *-*-* } }

const fn foo() {
    let c: i32 = 1i8; // { dg-error ".E0308." "" { target *-*-* } }
    let d: i8 = c; // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {
    let c: i32 = 1i8; // { dg-error ".E0308." "" { target *-*-* } }
    let d: i8 = c; // { dg-error ".E0308." "" { target *-*-* } }
}

