fn main() {
    let x: typeof(92) = 92; // { dg-error ".E0516." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

