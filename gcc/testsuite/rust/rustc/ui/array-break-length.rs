fn main() {
    loop {
        |_: [_; break]| {} // { dg-error ".E0268." "" { target *-*-* } }
    }

    loop {
        |_: [_; continue]| {} // { dg-error ".E0268." "" { target *-*-* } }
    }
}

