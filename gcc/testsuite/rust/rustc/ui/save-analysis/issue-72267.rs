// compile-flags: -Z save-analysis

fn main() {
    let _: Box<(dyn ?Sized)>;
// { dg-error ".E0224." "" { target *-*-* } .-1 }
// { dg-error ".E0224." "" { target *-*-* } .-2 }
}

