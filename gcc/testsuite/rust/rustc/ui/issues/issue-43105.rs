fn xyz() -> u8 { 42 }

const NUM: u8 = xyz();
// { dg-error ".E0015." "" { target *-*-* } .-1 }
// { dg-error ".E0015." "" { target *-*-* } .-2 }

fn main() {
    match 1 {
        NUM => unimplemented!(),
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
        _ => unimplemented!(),
    }
}

