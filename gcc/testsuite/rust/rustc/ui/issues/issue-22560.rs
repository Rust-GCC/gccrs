trait Add<Rhs=Self> {
    type Output;
}

trait Sub<Rhs=Self> {
    type Output;
}

type Test = dyn Add + Sub;
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
// { dg-error ".E0191." "" { target *-*-* } .-3 }
// { dg-error ".E0191." "" { target *-*-* } .-4 }

fn main() { }

