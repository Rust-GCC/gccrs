fn main() {
    if true /*!*/ {}
// { dg-error ".E0753." "" { target *-*-* } .-1 }
// { dg-error ".E0753." "" { target *-*-* } .-2 }
}

