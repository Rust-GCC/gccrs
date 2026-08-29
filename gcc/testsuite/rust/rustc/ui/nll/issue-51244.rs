fn main() {
    let ref my_ref @ _ = 0;
    *my_ref = 0;
// { dg-error ".E0594." "" { target *-*-* } .-1 }
}

