fn foo() -> isize { 23 }

static a: [isize; 2] = [foo(); 2];
// { dg-error ".E0015." "" { target *-*-* } .-1 }

fn main() {}

