// { dg-additional-options "-frust-unused-check-2.0" }
const A: usize = 1;
// { dg-warning "deadcode const item .A." "" { target *-*-* } .-1 }
