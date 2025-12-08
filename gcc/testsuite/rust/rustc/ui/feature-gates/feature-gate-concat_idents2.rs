fn main() {
    concat_idents!(a, b); // { dg-error ".E0425." "" { target *-*-* } }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
}

