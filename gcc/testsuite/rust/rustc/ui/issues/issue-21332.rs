struct S;

impl Iterator for S {
    type Item = i32;
    fn next(&mut self) -> Result<i32, i32> { Ok(7) }
// { dg-error ".E0053." "" { target *-*-* } .-1 }
// { dg-error ".E0053." "" { target *-*-* } .-2 }
}

fn main() {}

