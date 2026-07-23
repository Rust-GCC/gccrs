fn main() {
    |_:  [_; return || {}] | {};
// { dg-error ".E0572." "" { target *-*-* } .-1 }

    [(); return || {}];
// { dg-error ".E0572." "" { target *-*-* } .-1 }

    [(); return |ice| {}];
// { dg-error ".E0572." "" { target *-*-* } .-1 }

    [(); return while let Some(n) = Some(0) {}];
// { dg-error ".E0572." "" { target *-*-* } .-1 }
}

