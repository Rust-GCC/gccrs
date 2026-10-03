fn f1<'a>(x: u8, y: &'a ...) {}
// { dg-error ".E0743." "" { target *-*-* } .-1 }

fn f2<'a>(x: u8, y: Vec<&'a ...>) {}
// { dg-error ".E0743." "" { target *-*-* } .-1 }

fn main() {
    let _recovery_witness: () = 0; // { dg-error ".E0308." "" { target *-*-* } }
}

