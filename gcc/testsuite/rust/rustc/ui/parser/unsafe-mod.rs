unsafe mod m {
// { dg-error "" "" { target *-*-* } .-1 }
}

unsafe mod n;
// { dg-error ".E0583." "" { target *-*-* } .-1 }
// { dg-error ".E0583." "" { target *-*-* } .-2 }

fn main() {}

