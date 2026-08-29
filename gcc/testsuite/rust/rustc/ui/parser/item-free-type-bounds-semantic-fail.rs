fn main() {}

fn semantics() {
    type A: Ord;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    type B: Ord = u8;
// { dg-error "" "" { target *-*-* } .-1 }
    type C: Ord where 'static: 'static = u8;
// { dg-error "" "" { target *-*-* } .-1 }
    type D<_T>: Ord;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
    type E<_T>: Ord = u8;
// { dg-error ".E0091." "" { target *-*-* } .-1 }
// { dg-error ".E0091." "" { target *-*-* } .-2 }
    type F<_T>: Ord where 'static: 'static = u8;
// { dg-error ".E0091." "" { target *-*-* } .-1 }
// { dg-error ".E0091." "" { target *-*-* } .-2 }
}

