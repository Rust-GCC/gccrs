fn main() {
    // Tests case where inference fails due to the order in which casts are checked.
    // Ideally this would compile, see #48270.
    let x = &"hello";
    let mut y = 0 as *const _;
// { dg-error ".E0641." "" { target *-*-* } .-1 }
    y = x as *const _;
}

