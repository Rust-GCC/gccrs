// { dg-additional-options "-frust-unused-check-2.0" }
static A: usize = 1;
// { dg-warning "deadcode static item .A." "" { target *-*-* } .-1 }
