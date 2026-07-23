struct S {
    a: u8,
}

union U {
    a: u8,
}

fn main() {
    unsafe {
        let mut s: S;
        let mut u: U;
        s.a = 0; // { dg-error ".E0381." "" { target *-*-* } }
        u.a = 0; // { dg-error ".E0381." "" { target *-*-* } }
        let sa = s.a;
        let ua = u.a;
    }
}

