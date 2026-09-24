trait Trait {}

pub fn main() {
    let x: Vec<dyn Trait + Sized> = Vec::new();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
}

