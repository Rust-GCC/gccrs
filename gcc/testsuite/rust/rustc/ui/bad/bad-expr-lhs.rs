fn main() {
    1 = 2; // { dg-error ".E0070." "" { target *-*-* } }
    1 += 2; // { dg-error ".E0070." "" { target *-*-* } }
    (1, 2) = (3, 4); // { dg-error ".E0070." "" { target *-*-* } }
// { dg-error ".E0070." "" { target *-*-* } .-2 }
// { dg-error ".E0070." "" { target *-*-* } .-3 }

    let (a, b) = (1, 2);
    (a, b) = (3, 4); // { dg-error ".E0658." "" { target *-*-* } }

    None = Some(3); // { dg-error ".E0070." "" { target *-*-* } }
}

