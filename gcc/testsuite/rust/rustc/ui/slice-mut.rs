// Test mutability and slicing syntax.

fn main() {
    let x: &[isize] = &[1, 2, 3, 4, 5];
    // Immutable slices are not mutable.

    let y: &mut[_] = &x[2..4];
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

