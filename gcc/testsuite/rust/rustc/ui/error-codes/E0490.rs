fn f<'a, 'b>(y: &'b ()) {
    let x: &'a _ = &y;
// { dg-error ".E0495." "" { target *-*-* } .-1 }
// { dg-error ".E0495." "" { target *-*-* } .-2 }
// { dg-error ".E0495." "" { target *-*-* } .-3 }
}

fn main() {}

