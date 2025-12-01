fn main() {
    let mut _a = 3;
    let b = &mut _a;
    {
        let c = &*b;
        _a = 4; // { dg-error ".E0506." "" { target *-*-* } }
        drop(c);
    }
    drop(b);
}

