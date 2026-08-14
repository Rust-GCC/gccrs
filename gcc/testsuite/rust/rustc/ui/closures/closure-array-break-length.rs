fn main() {
    |_: [_; continue]| {}; // { dg-error ".E0268." "" { target *-*-* } }

    while |_: [_; continue]| {} {} // { dg-error ".E0268." "" { target *-*-* } }

    while |_: [_; break]| {} {} // { dg-error ".E0268." "" { target *-*-* } }
}

