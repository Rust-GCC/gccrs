fn f(a: u16, b: &str) {}

fn f2(a: u16) {}

fn main() {
    f(0);
// { dg-error ".E0061." "" { target *-*-* } .-1 }
// { dg-error ".E0061." "" { target *-*-* } .-2 }

    f2();
// { dg-error ".E0061." "" { target *-*-* } .-1 }
// { dg-error ".E0061." "" { target *-*-* } .-2 }
}

