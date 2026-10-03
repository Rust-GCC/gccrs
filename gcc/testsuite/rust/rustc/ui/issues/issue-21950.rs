trait Add<Rhs=Self> {
    type Output;
}

impl Add for i32 {
    type Output = i32;
}

fn main() {
    let x = &10 as &dyn Add;
// { dg-error ".E0191." "" { target *-*-* } .-1 }
// { dg-error ".E0191." "" { target *-*-* } .-2 }
}

