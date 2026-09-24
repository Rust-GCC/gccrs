extern {
    static mut a: i32;
}

fn main() {
    a += 3;     // { dg-error ".E0133." "" { target *-*-* } }
    a = 4;      // { dg-error ".E0133." "" { target *-*-* } }
    let _b = a; // { dg-error ".E0133." "" { target *-*-* } }
}

