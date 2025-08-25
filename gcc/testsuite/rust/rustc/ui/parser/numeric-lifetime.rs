struct S<'1> { s: &'1 usize }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
fn main() {
    // verify that the parse error doesn't stop type checking
    let x: usize = "";
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

