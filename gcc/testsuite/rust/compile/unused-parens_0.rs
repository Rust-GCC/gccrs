// { dg-additional-options "-frust-unused-check-2.0" }
pub fn a() {
    let _a = (true) && (false);
    let _b = (((2) + 1));
// { dg-warning "unnecessary parentheses" "" { target *-*-* } .-1 }
    let _c = ((((1))));
// { dg-warning "unnecessary parentheses" "" { target *-*-* } .-1 }
    let _d = (3 << 2) + 1;
}

