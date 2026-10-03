static mut a: isize = 3;

fn main() {
    a += 3;         // { dg-error ".E0133." "" { target *-*-* } }
    a = 4;          // { dg-error ".E0133." "" { target *-*-* } }
    let _b = a;     // { dg-error ".E0133." "" { target *-*-* } }
}

