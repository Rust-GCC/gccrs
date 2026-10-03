enum P {
    C(PC),
}

enum PC {
    Q,
    QA,
}

fn test(proto: P) {
    match proto { // { dg-error ".E0004." "" { target *-*-* } }
        P::C(PC::Q) => (),
    }
}

fn main() {}

