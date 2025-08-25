fn main() {}

extern {
    const A: isize;
// { dg-error "" "" { target *-*-* } .-1 }
    const B: isize = 42;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

