trait A {
    type Output;
    fn a(&self) -> <Self as A>::X;
// { dg-error ".E0576." "" { target *-*-* } .-1 }
}

impl A for u32 {
    type Output = u32;
    fn a(&self) -> u32 {
        0
    }
}

fn main() {
    let a: u32 = 0;
    let b: u32 = a.a();
}

