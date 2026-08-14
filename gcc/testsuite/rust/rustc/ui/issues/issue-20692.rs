trait Array: Sized + Copy {}

fn f<T: Array>(x: &T) {
    let _ = x
// { dg-error ".E0038." "" { target *-*-* } .-1 }
    as
    &dyn Array;
// { dg-error ".E0038." "" { target *-*-* } .-1 }
}

fn main() {}

